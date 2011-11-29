/*
 * @video_stage.c
 * @author marc-olivier.dzeukou@parrot.com
 * @author modified by Andrew D. Yates
 * @date 2007/07/27
 *
 * ihm vision thread implementation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <sys/time.h>
#include <time.h>

#include <VP_Api/vp_api.h>
#include <VP_Api/vp_api_error.h>
#include <VP_Api/vp_api_stage.h>
#include <VP_Api/vp_api_picture.h>
#include <VP_Stages/vp_stages_io_file.h>
#include <VP_Stages/vp_stages_i_camif.h>

#include <config.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Os/vp_os_malloc.h>
#include <VP_Os/vp_os_delay.h>
#include <VP_Stages/vp_stages_yuv2rgb.h>
#include <VP_Stages/vp_stages_buffer_to_picture.h>
#include <VLIB/Stages/vlib_stage_decode.h>

#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/Com/config_com.h>

#ifndef RECORD_VIDEO
#define RECORD_VIDEO
#endif
#ifdef RECORD_VIDEO
#    include <ardrone_tool/Video/video_stage_recorder.h>
#endif

#include <ardrone_tool/Video/video_com_stage.h>

#include "Video/video_stage.h"
#include "../UI/gui.h"

#include <ardrone_tool/UI/ardrone_input.h>
#include <stdio.h>

#include "../project/ppm.h"
#include "../project/camshift.h"
#include "../project/hsv.h"
#include "../project/frame.h"
#include "../project/fly.h"

#define NB_STAGES 10

PIPELINE_HANDLE pipeline_handle;

static uint8_t*  pixbuf_data       = NULL;
static vp_os_mutex_t  video_update_lock = PTHREAD_MUTEX_INITIALIZER;

C_RESULT output_gtk_stage_open( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  return (SUCCESS);
}

C_RESULT output_gtk_stage_transform( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  static int frame = 0;
  static int mass = 0, x_center = -1, y_center = -1;
  unsigned char mask_buf[WIDTH*HEIGHT];
  uint8_t display_data[WIDTH*HEIGHT*3];
  int is_hover;
  static int width, height;
  static int is_face = 0;
  int camshift_error;
  static float this_hue_buf[WIDTH*HEIGHT];
  static float last_hue_buf[WIDTH*HEIGHT];
  int i;
  static camshift_frames = 0;
  FILE *fp;
  char filename[50];
  
  frame++;

  // GET FRAME FROM VIDEO FEED
  vp_os_mutex_lock(&video_update_lock);
  pixbuf_data      = (uint8_t*)in->buffers[0];
  vp_os_mutex_unlock(&video_update_lock);

  // Process frame for orange ball
  // UNCOMMENT TO DETECT BALL
  //process_frame_ball(pixbuf_data, mask_buf, &mass, &x_center, &y_center);
  
  // UNCOMMENT TO DETECT FACES
  if (is_face == 0) {
    //printf("Detecting face...\n");
    process_frame_face(pixbuf_data, &mass, &x_center, &y_center, &width, &height);
    if (mass > 8) {
      is_face = 1;
      rgb2hue(pixbuf_data, last_hue_buf);
      printf("DETECT!!! x,y = %d, %d\n", x_center, y_center);
    }
  } else {
    rgb2hue(pixbuf_data, this_hue_buf);
    camshift_error = camshift(last_hue_buf, this_hue_buf, &x_center, &y_center, width, height);
    printf("camshift x,y = %d, %d\n", x_center, y_center);

    // copy this buffer to last buffer
    for (i=0; i<WIDTH*HEIGHT; i++) {
      last_hue_buf[i] = this_hue_buf[i];
    }
    
    if (camshift_error) {
      printf("CAMSHIFT ERROR\n");
      is_face = 0;
      x_center = -1;
      y_center = -1;
    }
    camshift_frames++;
    // about two seconds
    if (camshift_frames > 30) {
      printf("!!check to see if we are still tracking\n");
      camshift_frames = 0;
      is_face = 0;
    }
  }
  
  // we did not use the mask, so make it all black
  clear_mask(mask_buf);

  // write pixbuf to file
  sprintf(filename, "/home/a/pixbuf/pixbuf_%d.ppm", frame);
  fp = fopen(filename, "w");
  // write header
  fprintf(fp, "P6\n320 240\n255\n");
  for (i=0; i<320*240*3; i++) {
    fprintf(fp, "%c", pixbuf_data[i]);
  }
  fclose(fp);
  
  fp = fopen("/home/a/meanshift_log.txt", "a");
  // write log
  // framenum, mass, centroid_x, centroid_y, width, height
  fprintf(fp, "%d %d %d %d %d %d\n", frame, mass, x_center, y_center, width, height);
  
  // Get mask display
  display_mask(mask_buf, display_data, x_center, y_center);
  
  // Fly drone
  is_hover = fly(x_center, y_center, mass);

  // Print status
  printf("Frame number: %d\n", frame);
  printf("Mass: %d; Centroid: (%d, %d)\n", mass, x_center, y_center);
  printf("Meanshift Width: %d; Height: %d\n", width, height);
  printf("\033[2J");

  gdk_threads_enter();
  // GdkPixbuf structures to store the displayed picture
  static GdkPixbuf *pixbuf = NULL;
  static GdkPixbuf *maskbuf = NULL;

  if (pixbuf != NULL) {
    g_object_unref(pixbuf);
    pixbuf=NULL;
  }

  // Create GdkPixbuf from color frame data
  pixbuf = gdk_pixbuf_new_from_data(pixbuf_data,
  				    GDK_COLORSPACE_RGB,
  				    FALSE,    // No alpha channel
  				    8,        // 8 bits per pixel
  				    320,      // Image width
				    240,
  				    320 * 3,  // new pixel every 3 bytes (3channel per pixel)
  				    NULL,     // Function pointers
  				    NULL);
  // Create the GdkPixbuf from color mask display buffer
  maskbuf = gdk_pixbuf_new_from_data(display_data,
  				    GDK_COLORSPACE_RGB,
  				    FALSE,    // No alpha channel
  				    8,        // 8 bits per pixel
  				    320,      // Image width
				    240,
  				    320 * 3,  // new pixel every 3 bytes (3channel per pixel)
  				    NULL,     // Function pointers
  				    NULL);
  gui_t *gui = get_gui();

 // Display the image
  if (gui && gui->cam) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(gui->cam), pixbuf);
    // also display the pixbuf in a second window
    if (gui->mask_cam) {
      gtk_image_set_from_pixbuf(GTK_IMAGE(gui->mask_cam), maskbuf);
    }
  }
  gdk_threads_leave();
  
  return (SUCCESS);
}

C_RESULT output_gtk_stage_close( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  return (SUCCESS);
}


const vp_api_stage_funcs_t vp_stages_output_gtk_funcs =
{
  NULL,
  (vp_api_stage_open_t)output_gtk_stage_open,
  (vp_api_stage_transform_t)output_gtk_stage_transform,
  (vp_api_stage_close_t)output_gtk_stage_close
};

DEFINE_THREAD_ROUTINE(video_stage, data)
{
  C_RESULT res;

  vp_api_io_pipeline_t    pipeline;
  vp_api_io_data_t        out;
  vp_api_io_stage_t       stages[NB_STAGES];

  vp_api_picture_t picture;

  video_com_config_t              icc;
  vlib_stage_decoding_config_t    vec;
  vp_stages_yuv2rgb_config_t      yuv2rgbconf;
#ifdef RECORD_VIDEO
  video_stage_recorder_config_t   vrc;
#endif
  /// Picture configuration
  picture.format        = PIX_FMT_YUV420P;

  picture.width         = QVGA_WIDTH;
  picture.height        = QVGA_HEIGHT;
  picture.framerate     = 30;

  picture.y_buf   = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT     );
  picture.cr_buf  = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT / 4 );
  picture.cb_buf  = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT / 4 );

  picture.y_line_size   = QVGA_WIDTH;
  picture.cb_line_size  = QVGA_WIDTH / 2;
  picture.cr_line_size  = QVGA_WIDTH / 2;

  vp_os_memset(&icc,          0, sizeof( icc ));
  vp_os_memset(&vec,          0, sizeof( vec ));
  vp_os_memset(&yuv2rgbconf,  0, sizeof( yuv2rgbconf ));

  icc.com                 = COM_VIDEO();
  icc.buffer_size         = 100000;
  icc.protocol            = VP_COM_UDP;
  COM_CONFIG_SOCKET_VIDEO(&icc.socket, VP_COM_CLIENT, VIDEO_PORT, wifi_ardrone_ip);

  vec.width               = QVGA_WIDTH;
  vec.height              = QVGA_HEIGHT;
  vec.picture             = &picture;
  vec.block_mode_enable   = TRUE;
  vec.luma_only           = FALSE;

  yuv2rgbconf.rgb_format = VP_STAGES_RGB_FORMAT_RGB24;
#ifdef RECORD_VIDEO
  vrc.fp = NULL;
#endif

  pipeline.nb_stages = 0;

  stages[pipeline.nb_stages].type    = VP_API_INPUT_SOCKET;
  stages[pipeline.nb_stages].cfg     = (void *)&icc;
  stages[pipeline.nb_stages].funcs   = video_com_funcs;

  pipeline.nb_stages++;

#ifdef RECORD_VIDEO
  stages[pipeline.nb_stages].type    = VP_API_FILTER_DECODER;
  stages[pipeline.nb_stages].cfg     = (void*)&vrc;
  stages[pipeline.nb_stages].funcs   = video_recorder_funcs;

  pipeline.nb_stages++;
#endif // RECORD_VIDEO
  stages[pipeline.nb_stages].type    = VP_API_FILTER_DECODER;
  stages[pipeline.nb_stages].cfg     = (void*)&vec;
  stages[pipeline.nb_stages].funcs   = vlib_decoding_funcs;

  pipeline.nb_stages++;

  stages[pipeline.nb_stages].type    = VP_API_FILTER_YUV2RGB;
  stages[pipeline.nb_stages].cfg     = (void*)&yuv2rgbconf;
  stages[pipeline.nb_stages].funcs   = vp_stages_yuv2rgb_funcs;

  pipeline.nb_stages++;

  stages[pipeline.nb_stages].type    = VP_API_OUTPUT_SDL;
  stages[pipeline.nb_stages].cfg     = NULL;
  stages[pipeline.nb_stages].funcs   = vp_stages_output_gtk_funcs;

  pipeline.nb_stages++;

  pipeline.stages = &stages[0];
 
  /* Processing of a pipeline */
  if( !ardrone_tool_exit() )
  {
    PRINT("\n   Video stage thread initialisation\n\n");

    res = vp_api_open(&pipeline, &pipeline_handle);

    if( SUCCEED(res) )
    {
      int loop = SUCCESS;
      out.status = VP_API_STATUS_PROCESSING;

      while( !ardrone_tool_exit() && (loop == SUCCESS) )
      {
          if( SUCCEED(vp_api_run(&pipeline, &out)) ) {
            if( (out.status == VP_API_STATUS_PROCESSING || out.status == VP_API_STATUS_STILL_RUNNING) ) {
              loop = SUCCESS;
            }
          }
          else loop = -1; // Finish this thread
      }

      vp_api_close(&pipeline, &pipeline_handle);
    }
  }

  PRINT("   Video stage thread ended\n\n");

  return (THREAD_RET)0;
}

