#include "ppm.h"
#include <math.h>
#include <stdio.h>

// computed in matlab for sigma = 2
const float gauss2sig[13] = {0.0022, 0.0088, 0.0270, 0.0648, 0.1210, 0.1760, 0.1995, 0.1760, 0.1210, 0.0648, 0.0270, 0.0088, 0.0022};

void gauss_blur(unsigned char* buf) {
  /* Apply 2 sigma gaussian blur to rgb buffer. 
     Args:
       buf: rgb byte buffer to blur
  */
  static float xbuf[WIDTH*HEIGHT*3];
  
  int x, y, a;
  int i, k;
  float sum;
  // compute x gaussians for all three channels into buffer
  for (a=0; a<3; a++) {
    for (x=0; x<WIDTH; x++) {
      for (y=0; y<HEIGHT; y++) {
	sum = 0;
	for (k=0; k<13; k++) {
	  i = ind3(x+k-6,y,a);
	  sum += gauss2sig[k] * buf[i];
	}
	i = ind3(x,y,a);
	xbuf[i] = sum;
      }
    }
  }
  // compute y gaussian from x gaussian and save in original buffer
  for (a=0; a<3; a++) {
    for (x=0; x<WIDTH; x++) {
      for (y=0; y<HEIGHT; y++) {
	sum = 0;
	for (k=0; k<13; k++) {
	  i = ind3(x,y+k-6,a);
	  sum += gauss2sig[k] * xbuf[i];
	}
	i = ind3(x,y,a);
	buf[i] = round(sum);
      }
    }
  }

}

