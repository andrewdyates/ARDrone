#include "ppm.h"
#include <stdio.h>

int main() {
  unsigned char buf[WIDTH*HEIGHT*3];
  char *filename_in;
  char *filename_out;
  unsigned char c1, c2, c3;
  filename_in = "pixbuff_7.ppm";
  filename_out = "outtest.ppm";
  read_ppm(buf, filename_in);
  write_ppm(buf, filename_out);
  // verify index function works as expected
  c1 = ind3(buf, 0, 0, 0);
  c2 = ind3(buf, 1, 0, 0);
  c3 = ind3(buf, 1, 1, 2);
  // should be 102, 102, 79
  printf("%d %d %d\n", c1, c2, c3);
  return 0;
}
  
