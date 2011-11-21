#include "ppm.h"
#include <stdio.h>

int main() {
  unsigned char buf[WIDTH*HEIGHT*3];
  char *filename_in;
  char *filename_out;
  unsigned char c1, c2, c3;
  filename_in = "testdata/pixbuff_7.ppm";
  filename_out = "testdata/outtest.ppm";
  read_ppm(buf, filename_in);
  write_ppm(buf, filename_out);
  // verify index function works as expected
  c1 = buf[ind3(0, 0, 0)];
  c2 = buf[ind3(1, 0, 0)];
  c3 = buf[ind3(1, 1, 2)];
  printf("should be 102, 102, 79\n");
  printf("%d %d %d\n", c1, c2, c3);
  return 0;
}
  
