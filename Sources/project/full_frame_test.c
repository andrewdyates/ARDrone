#include "gauss.h"
#include "hsv.h"
#include "binfilter.h"
#include "threshold.h"
#include "ppm.h"
#include "moments.h"
#include <stdio.h>

/* simulate processing for one complete frame cycle */
int main(int argc, char* argv[]) {
  
  unsigned char rgb_buf[WIDTH*HEIGHT*3];
  float hsv_buf[WIDTH*HEIGHT*3];
  unsigned char mask_buf[WIDTH*HEIGHT];
  int mass, x, y;
  
  if (argc < 2) {
    printf("Usage: exe filename.ppm\n");
    return 1;
  }

  // read frame
  read_ppm(rgb_buf, argv[1]);
  // blur
  gauss_blur(rgb_buf);
  // convert to hue, saturation, value
  rgb2hsv(rgb_buf, hsv_buf);
  // convert to mask with orange threshold 
  orange_thresh(hsv_buf, mask_buf);
  // apply 3x3 median filter
  median_filter(mask_buf);
  // compute mass and centroid
  mass = centroid(mask_buf, &x, &y);
  printf("Mass: %d; ", mass);
  if (mass) {
    printf("Centroid: (%d, %d)\n", x, y);
  } else {
    printf("No centroid\n");
  }
  // output mask for verification
  write_ppm1(mask_buf, "testdata/full_frame_test.ppm");
  printf("Wrote mask to 'testdata/full_frame_test.ppm' for verification.\n");

  return 0;
}

