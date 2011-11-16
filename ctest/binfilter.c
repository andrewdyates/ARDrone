#include "binfilter.h"
#include "ppm.h"

void median_filter(unsigned char* buf) {
  /* Apply 3x3 median filter to WIDTH*HEIGHT binary image mask buf */
  int x, y, i;
  int ones;
  int xx, offset;
  unsigned char rowbuf[WIDTH*2];
  
  for (x=0; x<WIDTH; x++) {
    for (y=0; y<HEIGHT; y++) {
      offset = WIDTH*(y%2);
      // write row from two iterations ago
      if (y>=2) {
	for (xx=0; x<WIDTH; x++) {
	  buf[ind1(xx,y-2)] = rowbuf[x+offset];
	}
      }
      // compute median from sum of 0s and 1s
      ones =  buf[ind1(x-1,y-1)] + buf[ind1(x,y-1)] + buf[ind1(x+1,y-1)];
      ones += buf[ind1(x-1,y)] + buf[ind1(x,y)] + buf[ind1(x+1,y)];
      ones += buf[ind1(x-1,y+1)] + buf[ind1(x,y+1)] + buf[ind1(x+1,y+1)];
      if (ones >= 5) {
	rowbuf[x + offset] = 1;
      } else {
	rowbuf[x + offset] = 0;
      }
    }
  }
  // write last two rows
  offset = WIDTH*(y%2);
  for (xx=0; x<WIDTH; x++) {
    buf[ind1(xx,y-2)] = rowbuf[x+offset];
  }
  y++;
  offset = WIDTH*(y%2);
  for (xx=0; x<WIDTH; x++) {
    buf[ind1(xx,y-2)] = rowbuf[x+offset];
  }
      

}
