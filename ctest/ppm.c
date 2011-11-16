#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>

int read_ppm(unsigned char* buf, char* filename) {
  /* Load a 320x240 RGB P6 PPM file into a buffer. 

     Args:
       buf: preallocated byte buffer of size 320*240*3
       filename: .ppm file to load
     Returns:
       0: success
       -1: I/O error
   */
  FILE *fp;
  int i;
  unsigned char c;
  int num_bytes = 80;
  char header[num_bytes];
  char *r;

  // open filename
  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "WARNING: Cannot open file '%s' for read.", filename);
    return -1;
  }
  // ignore first three lines
  r = fgets(header, num_bytes, fp);
  r = fgets(header, num_bytes, fp);
  r = fgets(header, num_bytes, fp);
  // read bytes
  for (i=0; i<WIDTH*HEIGHT*3; i++) {
    c = fgetc(fp);
    if (!feof(fp) && !ferror(fp)) {
      buf[i] = c;
    } else {
      fprintf(stderr, "WARNING: EOF or ERROR encountered unexpectedly\n");
      return -1;
    }
  }
  fclose(fp);
  return 0;
}

int write_ppm(unsigned char* buf, char* filename) {
  /* Write a 320x240 RGB buffer to as a P6 PPM file. 

     Args:
       buf: preallocated byte buffer of size 320*240*3
       filename: file name to which to write
     Returns:
       0: success
       -1: I/O error
   */
  FILE *fp;
  int i;

  fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "WARNING: Cannot open file '%s' for write.", filename);
    return -1;
  }

  // write header
  fprintf(fp, "P6\n320 240\n255\n");
  // write bytes
  for (i=0; i<WIDTH*HEIGHT*3; i++) {
    fputc(buf[i], fp);
  }
  fclose(fp);
  return 0;
}


unsigned int ind3(int x, int y, char a) {
  /* Index 3 channel byte buffer: Return index for pixel at (x,y,a)
     Access Out-of-Bounds indices at nearest boundary index.

  Args:
    x: 0 to WIDTH-1 x coordinate of pixel
    y: 0 to HEIGHT-1 y coordinate of pixel
    a: offset 0 to 2 (rgb or hsv)
  Returns:
    index value of (x,y,a)
  */
  unsigned int i;
  if (x < 0) x = 0;
  if (x > WIDTH-1) x = WIDTH-1;
  if (y < 0) y = 0;
  if (y > HEIGHT-1) y = HEIGHT-1;
  if (a < 0) a = 0;
  if (a > 2) a = 2;
  i = y*WIDTH*3 + x*3 + a;
  return i;
}

unsigned int ind1(int x, int y) {
  /* Index 1 channel byte buffer: Return index for pixel at (x,y)
     Access Out-of-Bounds indices at nearest boundary index.

  Args:
    x: 0 to WIDTH-1 x coordinate of pixel
    y: 0 to HEIGHT-1 y coordinate of pixel
  Returns:
    index value of (x,y)
  */
  unsigned int i;
  i = ind3(x, y, 0) / 3;
  return i;
}

int write_ppm1(unsigned char* buf, char* filename) {
  /* Write a single channel 320x240 mask buffer to as a P1 PPM file. 

     Args:
       buf: preallocated byte buffer of size 320*240
       filename: file name to which to write
     Returns:
       0: success
       -1: I/O error
   */
  FILE *fp;
  int x, y;

  fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "WARNING: Cannot open file '%s' for write.", filename);
    return -1;
  }

  // write header
  fprintf(fp, "P1\n320 240\n");
  // write bytes
  int a;
  for (y=0; y<HEIGHT; y++) {
    for (x=0; x<WIDTH-1; x++) {
      fprintf(fp, "%d ", buf[ind1(x,y)]);
    }
    fprintf(fp, "%d\n", buf[ind1(x+1,y)]);
  }
  fclose(fp);
  return 0;
}


int read_ppm1(unsigned char* buf, char* filename) {
  /* Load a 320x240 mask P1 PPM file into a buffer. 

     Args:
       buf: preallocated byte buffer of size 320*240
       filename: .ppm file to load
     Returns:
       0: success
       -1: I/O error
   */
  FILE *fp;
  int i;
  unsigned char c;
  int num_bytes = 80;
  char header[num_bytes];
  char *r;

  // open filename
  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "WARNING: Cannot open file '%s' for read.", filename);
    return -1;
  }
  // ignore first two lines
  r = fgets(header, num_bytes, fp);
  r = fgets(header, num_bytes, fp);
  // read bytes
  for (i=0; i<WIDTH*HEIGHT; i++) {
    c = fgetc(fp);
    fgetc(fp); // consume whitespace
    if (!feof(fp) && !ferror(fp)) {
      buf[i] = c - '0';
    } else {
      fprintf(stderr, "WARNING: EOF or ERROR encountered unexpectedly\n");
      return -1;
    }
  }
  fclose(fp);
  return 0;
}
