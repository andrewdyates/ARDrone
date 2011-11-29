/*
 *  camshift.h
 *  cshift
 *
 *  Created by daniya on 11/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

int camshift(float* im1, float* im2, int* cx, int* cy, int ww, int wh);
void constructHist(unsigned int* hist, float* im, int cx, int cy, int ww, int wh);
void backprojectHist(unsigned int* H, float* im, unsigned int* probim);
int mshift(unsigned int* probim, int* cx, int* cy, int ww, int wh, double *m00);
double compute_m00(unsigned int* probim, int cx, int cy, int ww, int wh);
double compute_m10(unsigned int* probim, int cx, int cy, int ww, int wh);
double compute_m01(unsigned int* probim, int cx, int cy, int ww, int wh);
int findMax(unsigned int* hist);
int findMin(unsigned int* hist);
