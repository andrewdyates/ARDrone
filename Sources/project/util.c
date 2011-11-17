#include <stdarg.h>
#include "math.h"

int minimum(int n, ...) {
  /* Return minimum of series of integers. */
  register int i;
  int min, a;
  va_list ap;

  va_start(ap, n);
  min = va_arg(ap, int);
  for(i = 2; i <= n; i++) {
    a = va_arg(ap, int);
    if (a < min) {
      min = a;
    }
  }
  va_end(ap);
  return min;
}

int maximum(int n, ...) {
  /* Return maximum of series of integers. */
  register int i;
  int max, a;
  va_list ap;

  va_start(ap, n);
  max = va_arg(ap, int);
  for(i = 2; i <= n; i++) {
    a = va_arg(ap, int);
    if (a > max) {
      max = a;
    }
  }
  va_end(ap);
  return max;
}

long intpow(int a, int p) {
  /* integer power.
  a: base
  p: power >= 0
  */
  long ans = 1;
  int mask = 1;
  int q = p;
  // compute mask size
  while (q) {
    q >>= 1;
    mask <<= 1;
  }
  mask >>= 1;
  
  while (mask) {
    if ((p & mask) != 0) {
      ans = ans * ans * a;
    } else {
      ans = ans * ans;
    }
    mask >>= 1;
  }
  return ans;
}
