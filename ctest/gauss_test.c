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
  printf("%d %d %d\n", buf[12], buf[13], buf[14]);
  gauss_blur(buf);
  printf("%d %d %d\n", buf[12], buf[13], buf[14]);
  write_ppm(buf, filename_out);
  return 0;
}
  
