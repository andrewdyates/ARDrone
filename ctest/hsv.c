#include "hsv.h"
#include "ppm.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>


void rgb2hsv(char* rgb_buf, float* hsv_buf) {
  /* Convert (red, green, blue) to (hue, saturation, value).

  Args:
    rgb_buf: read from WIDTH*HEIGHT*3 rgb buffer
    hsv_buf: write to WIDTH*HEIGHT*3 hsv buffer
    rgb_buf: WIDTH*HEIGHT*3 rgb buffer
  */
  register int x, y;
  unsigned char r, g, b;
  unsigned char max, min;
  float chroma, hue, saturation, value;

  for (x=0; x<WIDTH-1; x++) {
    for (y=0; y<HEIGHT-1; y++) {
      r = rgb_buf[ind3(x, y, 0)];
      g = rgb_buf[ind3(x, y, 1)];
      b = rgb_buf[ind3(x, y, 2)];
      max = maximum(3, r, g, b);
      min = minimum(3, r, g, b);
      chroma = max-min;
      // VALUE
      value = max / 255.0;
      // SATURATION
      if (value > 0) {
	saturation = chroma / max;
      } else {
	saturation = 0;
      }
      // HUE
      if (chroma == 0) {
	hue = 0.0;
      } else if (max == r) {
	hue = (g-b)/chroma;
	if (hue < 0) {
	  hue = hue + 6;
	}
      } else if (max == g) {
	hue = (b-r)/chroma + 2;
      } else {
	hue = (r-g)/chroma + 4;
      }
      // scale to fraction of 360 degrees
      hue = hue / 6.0;

      // set pixel values in HSV buffer
      hsv_buf[ind3(x,y,0)] = hue;
      hsv_buf[ind3(x,y,1)] = saturation;
      hsv_buf[ind3(x,y,2)] = value;
    }

  }
      
}

