#include "ppm.h"
#include "gauss.h"
#include <stdio.h>

int main() {
  unsigned char buf[WIDTH*HEIGHT*3];
  char *filename_in;
  char *filename_out;
  filename_in = "pixbuff_7.ppm";
  filename_out = "blurtest.ppm";
  read_ppm(buf, filename_in);
  printf("These two sequences of numbers should be different.\n");
  printf("%d %d %d\n", buf[12], buf[13], buf[14]);
  gauss_blur(buf);
  printf("%d %d %d\n", buf[12], buf[13], buf[14]);
  write_ppm(buf, filename_out);
  printf("Confirm that the image blurtest.ppm is a blurred copy of image pixbuff_7.ppm\n");
  return 0;
}
  
