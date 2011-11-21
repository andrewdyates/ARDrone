// test hsv conversion

#include "ppm.h"
#include "hsv.h"

#include <stdio.h>

int main() {
  unsigned char rgb_buf[WIDTH*HEIGHT*3];
  float hsv_buf[WIDTH*HEIGHT*3];
  char *filename_in;
  char *filename_out;
  float f1, f2, f3, f4;
  FILE *fp;
  int i;
  
  filename_in = "testdata/pixbuff_7.ppm";
  filename_out = "testdata/pixbuff_7_hsv.txt";
  read_ppm(rgb_buf, filename_in);
  rgb2hsv(rgb_buf, hsv_buf);
  // verify sample hsv values
  f1 = hsv_buf[ind3(0, 0, 0)];
  f2 = hsv_buf[ind3(0, 0, 1)];
  f3 = hsv_buf[ind3(0, 0, 2)];
  f4 = hsv_buf[ind3(1, 1, 0)];

  printf("should be 53, 23, 40, 53\n");
  printf("%f %f %f %f\n", f1*360, f2*100, f3*100, f4*360);
  printf("Writing buffer for verification...\n");
  
  fp = fopen(filename_out, "w");
  for (i=0; i<WIDTH*HEIGHT*3; i++) {
    fprintf(fp, "%f\n", hsv_buf[i]);
  }
  fclose(fp);
  return 0;
}
  
