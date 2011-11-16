#include "moments.h"
#include "ppm.h"
#include "math.h"
#include "util.h"

long moment(unsigned char *buf, int xp, int yp) {
  /* Compute spacial moment of binary image mask mask.
  Args:
    buf: image buffer of WIDTH*HEIGHT binary image mask
    xp: power of x
    yp: power of y
 */
  long sum;
  int x, y, i;
  
  sum = 0;
  for (x=0; x<WIDTH; x++) {
    for (y=0; y<HEIGHT; y++) {
      i = ind1(x,y);
      // this is wrong!
      sum += intpow(x, xp) * intpow(y, yp) * buf[i];
    }
  }
  return sum;
}

long centroid(unsigned char *buf, int *xbar, int *ybar) {
  /* Compute (x,y) centroid of binary image, return if centroid exists.
  Args:
    buf: image buffer of WIDTH*HEIGHT binary image mask
    xbar: pointer to x coordinate of centroid
    ybar: pointer to y coordinate of centroid
  Returns:
     0: no mass, no centroid computed
     >0: mass of centroid computed
 */
  double m00, m10, m01;
  m00 = moment(buf, 0, 0);
  if (m00 == 0) {
    return 0;
  }
  m10 = moment(buf, 1, 0);
  m01 = moment(buf, 0, 1);
  *xbar = round(m10/m00);
  *ybar = round(m01/m00);
  return (long)m00;
}

