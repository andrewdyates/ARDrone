/*
 *  camshift.c
 *  cshift
 *
 *  Created by daniya on 11/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "camshift.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppm.h"

/*
im1 is a hue band of HSV image
im2 is a hue band of HSV image
 */
int camshift(float* im1, float* im2, int* cx, int* cy, int ww, int wh)
{
	unsigned int hist[256];
	unsigned int probim[WIDTH*HEIGHT];
	double m00;
	int progress;
	
	if(ww % 2) ww = ww - 1;
	if(wh % 2) wh = wh - 1;
	
	//create histogram
	constructHist(hist, im1, *cx, *cy, ww, wh);
	
	//backproject histogram
	backprojectHist(hist, im2, probim);
	
	//perform mean-shift
	progress = mshift(probim, cx, cy, ww, wh, &m00);
	if (m00 / (ww*wh) < 0.5) {
	  progress = 1;
	}
	if(progress) {
	  printf("image out of bounds!");
	  return 1;
	}
	
	return 0;
}//camshift

void constructHist(unsigned int* hist, float* im, int cx, int cy, int ww, int wh)
{
	int i,j;
	int val;
	
	for(i = 0; i < 256; i++)
		hist[i] = 0;
	
	for(j = cy-wh/2; j <= cy+wh/2; j++)
	{
		for(i = cx-ww/2; i <= cx+ww/2; i++)
		{
			val = round(im[ind1(i,j)]);
			hist[val]++;
		}//forj
	}//fori
}//constructHist

void backprojectHist(unsigned int* H, float* im, unsigned int* probim)
{
	int i,j, val;
	
	for(i = 0; i < WIDTH; i++)
		for(j = 0; j < HEIGHT; j++)
		{
			val = round(im[ind1(i,j)]);
			probim[ind1(i,j)] = H[val];
		}
}//backprojectHist

int mshift(unsigned int* probim, int* cx, int* cy, int ww, int wh, double* m00)
{
	int old_cx = *cx;
	int old_cy = *cy;
	*m00 = compute_m00(probim, old_cx, old_cy, ww, wh);
	*cx = round(compute_m10(probim, old_cx, old_cy, ww, wh) / *m00);
	*cy = round(compute_m01(probim, old_cx, old_cy, ww, wh) / *m00);
	
	if(*cx - ww/2 < 0 || *cy - wh/2 < 0 || *cx + ww/2 > WIDTH-1 || *cy + wh/2 > HEIGHT-1)
		return 1;
	
	return 0;
}//mshift

double compute_m00(unsigned int* probim, int cx, int cy, int ww, int wh)
{
	int i,j;
	double m = 0;
	
	for(j = cy-wh/2; j <= cy+wh/2; j++)
		for(i = cx-ww/2; i <= cx+ww/2; i++)
			m = m + probim[ind1(i,j)];
	
	return m;
}//m00

double compute_m10(unsigned int* probim, int cx, int cy, int ww, int wh)
{
	int i,j;
	double m = 0;
	
	for(j = cy-wh/2; j <= cy+wh/2; j++)
		for(i = cx-ww/2; i <= cx+ww/2; i++)
			m = m + i*probim[ind1(i,j)];
	
	return m;
}//m10

double compute_m01(unsigned int* probim, int cx, int cy, int ww, int wh)
{
	int i,j;
	double m = 0;
	
	for(j = cy-wh/2; j <= cy+wh/2; j++)
		for(i = cx-ww/2; i <= cx+ww/2; i++)
			m = m + j*probim[ind1(i,j)];
	
	return m;
}//m01
