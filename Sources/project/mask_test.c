#include "hsv.h"
#include "util.h"
#include "gauss.h"
#include "ppm.h"
#include "moments.h"
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
  
  // ==================
  // 1. Test Threshold
  // ==================
  // load ppm
  read_ppm(rgb_buf, argv[1]);
  // convert to hsv
  rgb2hsv(rgb_buf, hsv_buf);
  // get mask
  orange_thresh(hsv_buf, mask_buf);
  // write mask
  write_ppm1(mask_buf, "testdata/raw_mask.ppm");
  printf("Confirm that the binary image 'testadata/raw_mask.ppm' is a masked copy of rgb image '%s'.\n", argv[1]);

  // ==================
  // 2. Test Gaussian Blur
  // ==================
  // load ppm
  read_ppm(rgb_buf, argv[1]);
  gauss_blur(rgb_buf);
  // convert to hsv
  rgb2hsv(rgb_buf, hsv_buf);
  // get mask
  orange_thresh(hsv_buf, mask_buf);
  // write mask
  write_ppm1(mask_buf, "testdata/blur_mask.ppm");
  printf("Confirm that the binary image 'testdata/blur_mask.ppm' is a masked copy of blurred rgb image '%s'.\n", argv[1]);

  // ==================
  // 3. Test Centroid (given mask computed in "2. Test Gaussian Blur"
  // ==================
  
  long mass;
  int x_bar, y_bar;
  long m00;
  mass = centroid(mask_buf, &x_bar, &y_bar);
  if (mass > 0) {
    printf("Mass: %ld. Centroid (x,y): (%d, %d)\n", mass, x_bar, y_bar);
  } else {
    printf("No mass in mask. No centroid computed.\n");
  }
  m00 = moment(mask_buf, 0, 0);
  printf("m00 Mass is %ld\n", m00);

  // turn off centroid (display as block dot against presumably a white background
  mask_buf[ind1(x_bar, y_bar)] = 0;
  write_ppm1(mask_buf, "testdata/blur_mask_centroid.ppm");
  printf("Confirm that the binary image 'testdata/blur_mask_centroid.ppm' has a black pixel at the centroid.\n");
  
  return 0;
}
