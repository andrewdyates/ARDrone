#include "hsv.h"
#include "ppm.h"
#include "threshold.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: exec filename.ppm\n");
    return 1;
  }

  unsigned char rgb_buf[WIDTH*HEIGHT*3];
  float hsv_buf[WIDTH*HEIGHT*3];
  unsigned char mask_buf[WIDTH*HEIGHT];
  
  // load ppm
  read_ppm(rgb_buf, argv[1]);
  // convert to hsv
  rgb2hsv(rgb_buf, hsv_buf);
  // get mask
  orange_thresh(hsv_buf, mask_buf);
  // write mask
  write_ppm1(mask_buf, "mask.ppm");
  printf("Confirm that the binary image mask.ppm is a blurred copy of rgb image %s.\n", argv[1]);
  return 0;
}
