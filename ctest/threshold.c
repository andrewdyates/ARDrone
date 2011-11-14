#include "ppm.h"

void orange_thresh(float* fbuf, unsigned char* mask) {
  /* Find orange ball mask from HSV float image buffer. */
  /* Thresholds from 'ppm_read_orange.m' */
  float hue, sat, val;
  int i,x,y;
  
  for (x=0; x<WIDTH; x++) {
    for (y=0; y<HEIGHT; y++) {
      hue = fbuf[ind3(x,y,0)];
      sat = fbuf[ind3(x,y,1)];
      val = fbuf[ind3(x,y,2)];
      i = ind1(x, y);
      mask[i] = 0;
      // highlights values
      if (val >= .99 && (hue < 30.0/360.0 || hue > 355.0/360.0) && sat > .1) {
	mask[i] = 1;
      }
      // mid values
      if (val >= .2 && (hue > 5.0/360.0 && hue < 15.0/360.0) && sat > .55) {
	mask[i] = 1;
      }
      if ((hue < 25.0/360.0 || hue > 358.0/360.0) && sat > .6) {
	mask[i] = 1;
      }
      // shadows
      if (val < .6 && val > .3 && (hue < 20.0/360.0 || hue > 355.0/360.0) && sat > .65) {
	mask[i] = 1;
      }
    }
  }
}
