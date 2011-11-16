#include "util.h"
#include <stdio.h>

int main() {
  // confirm intpow
  printf("2^0 = %ld; 2^1 = %ld; 2^2 = %ld; 2^3 = %ld; 2^5 = %ld; 2^8 = %ld;\n", intpow(2,0), intpow(2,1), intpow(2,2), intpow(2,3), intpow(2,5), intpow(2,8));

  // confirm minimum
  printf("Minimum of 1, 4, -8, 1, 2 is: %d (-8)\n", minimum(5, 1, 4, -8, 1, 2));
  // confirm maximum  
  printf("Maximum of 1, 4, -8, 1, 2 is: %d (4)\n", maximum(5, 1, 4, -8, 1, 2));
    // confirm maximum  
  printf("Maximum of 1 is: %d (1)\n", maximum(1, 1));

  return 0;
}
