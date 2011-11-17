#include "frame.h"
#include "gauss.h"
#include "hsv.h"
#include "binfilter.h"
#include "threshold.h"
#include "ppm.h"
#include "moments.h"

#define MASS 6

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
  // compute mass and (sufficient) centroid
  *mass = centroid(mask_buf, x_center, y_center);
  if (*mass < MASS) {
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
  int off, i1, i2, j1, j2;
  
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
  // if centroid exists (is non-negative)
  if (x_center >= 0 && y_center >= 0) {
    // draw centroid as green square with blue center
    // "ind3" handles any overflow
    i = ind3(x_center,y_center,0);
    display_buf[i] = 0;         // red channel
    display_buf[i+1] = 0;       // green channel
    display_buf[i+2] = 255;     // blue channel
    // draw green crosshair if centroid exists
    for (off=4; off>0; off--) {
      i1 = ind3(x_center+off,y_center,0);
      i2 = ind3(x_center-off,y_center,0);
      j1 = ind3(x_center,y_center+off,0);
      j2 = ind3(x_center,y_center-off,0);
      // horizontal draw
      display_buf[i1] = 0;     // red channel
      display_buf[i1+1] = 255; // green channel
      display_buf[i1+2] = 0;   // blue channel
      display_buf[i2] = 0;     // red channel
      display_buf[i2+1] = 255; // green channel
      display_buf[i2+2] = 0;   // blue channel
      // vertical draw
      display_buf[j1] = 0;     // red channel
      display_buf[j1+1] = 255; // green channel
      display_buf[j1+2] = 0;   // blue channel
      display_buf[j2] = 0;     // red channel
      display_buf[j2+1] = 255; // green channel
      display_buf[j2+2] = 0;   // blue channel
    }
  } else {
    // draw red "x" in center if no object detected (assume all black)
    for (off=0; off<16; off++) {
      // center of frame is 320/2= 160, 240/2= 120
      i1 = ind3(160+off, 120+off, 0);
      i2 = ind3(160+off, 120-off, 0);
      j1 = ind3(160-off, 120+off, 0);
      j2 = ind3(160-off, 120-off, 0);
      display_buf[i1] = 255;
      display_buf[i2] = 255;
      display_buf[j1] = 255;
      display_buf[j2] = 255;
    }
  }
}

