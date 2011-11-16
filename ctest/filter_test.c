#include "ppm.h"
#include "binfilter.h"
#include <stdio.h>

int main() {
  unsigned char mask[WIDTH*HEIGHT];
  read_ppm1(mask, "noisy_mask.ppm");
  median_filter(mask);
  write_ppm1(mask, "filtered_mask.ppm");
  printf("Verify that 'filtered_mask.ppm' has been correctly filtered.\n");
  return 0;
}
