#include "frame.h"
#include "gauss.h"
#include "hsv.h"
#include "binfilter.h"
#include "threshold.h"
#include "ppm.h"
#include "moments.h"

void process_frame(unsigned char* frame_buf, unsigned char* mask_buf, int* mass, int* x_center, int* y_center) {
  /* Process one frame of RGB video. 
 
  Args:
    frame_buf: WIDTH*HEIGHT*3 color video frame
    mask_buf: WIDTH*HEIGHT binary mask of rgb_buf
    mass: number of pixels in mask
    x_center: x centroid cooridinate or -1 if none
    y_center: y centroid cooridinate or -1 if none
  */
  unsigned char rgb_buf[WIDTH*HEIGHT*3];
  float hsv_buf[WIDTH*HEIGHT*3];
  int i;
  
  // copy frame into working rgb buffer
  for (i=0; i<WIDTH*HEIGHT*3; i++) {
    rgb_buf[i] = frame_buf[i];
  }
  // blur
  gauss_blur(rgb_buf);
  // convert to hue, saturation, value
  rgb2hsv(rgb_buf, hsv_buf);
  // convert to mask with orange threshold 
  orange_thresh(hsv_buf, mask_buf);
  // apply 3x3 median filter
  median_filter(mask_buf);
  // compute mass and centroid
  *mass = centroid(mask_buf, x_center, y_center);
  if (mass == 0) {
    *x_center = -1;
    *y_center = -1;
  }
}

void display_mask(unsigned char* mask_buf, unsigned char* display_buf, int x_center, int y_center) {
  /* Display binary mask as color image with centroid.
     
   Args:
     mask_buf: WIDTH*HEIGHT binary mask image
     display_buf: WIDTH*HEIGHT*3 color image
     x_center: x coordinate of centroid or -1 if none
     y_center: y coordinate of centroid or -1 if none
   */
  int x, y, i;
  for (y=0; y<HEIGHT; y++) {
    for (x=0; x<WIDTH; x++) {
      i = ind3(x,y,0);
      if (mask_buf[ind1(x,y)]) {
	// mask is on: set pixel to white
	display_buf[i] = 255;
	display_buf[i+1] = 255;
	display_buf[i+2] = 255;
      } else {
	// mask is off: set pixel to black
	display_buf[i] = 0;
	display_buf[i+1] = 0;
	display_buf[i+2] = 0;
      }
    }
  }
  if (x_center >= 0 && y_center >= 0) {
    // draw centroid as green square with blue center
    // "ind3" handles any overflow
    i = ind3(x_center,y_center,0);
    display_buf[i] = 0;         // red channel
    display_buf[i+1] = 0;       // green channel
    display_buf[i+2] = 255;     // blue channel
    // draw green box
    for (y=y_center-1; y<=y_center+1; y++) {
      for (x=x_center-1; x<=x_center+1; x++) {
	i = ind3(x,y,0);
	display_buf[i] = 0;     // red channel
	display_buf[i+1] = 255; // green channel
	display_buf[i+2] = 0;   // blue channel
      }
    }
  }
}

