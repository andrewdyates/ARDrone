#include "facedetect.h"
#include "cv.h"
#include "../ardrone_testing_tool.h"

//int faceCount = 0;
//char faceImageName[512];

void DetectFace(unsigned char* frame_buf, int *center_x, int *center_y, int *width, int *height)
{
	IplImage *cvTempIm = cvCreateImage(cvSize(320,240),IPL_DEPTH_8U,3);
	IplImage *frame = cvCreateImage(cvSize(320,240),IPL_DEPTH_8U,3);

	cvTempIm->imageData = frame_buf;
	cvCvtColor(cvTempIm, frame, CV_BGR2RGB);
	cvReleaseImage(&cvTempIm);

	CvHaarClassifierCascade *frontCascade;
	CvMemStorage *frontStorage;
	if (GetCvClassifier(&frontCascade) == 1 || GetCvStorage(&frontStorage) == 1)
	{
		*center_x = -1;
		*center_y = -1;
		*width = 0;
		*height = 0;
		cvReleaseImage(&frame);
		printf("DIE\n");
		return;
	}

	CvRect *bounds;
	CvSeq *faces;
	cvClearMemStorage(frontStorage);

	faces = cvHaarDetectObjects(frame, frontCascade, frontStorage,
			1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
			cvSize(20, 20));
	
	printf("Face...\n");
	if (faces->total > 0)
	{
	  printf("YES FACE\n");
		bounds = (CvRect *)cvGetSeqElem(faces, 0);
		*center_x = (bounds->x + bounds->width / 2);
		*center_y = (bounds->y + bounds->height / 2);
		*width = bounds->width;
		*height = bounds->height;
		printf("in facedetect: %d %d\n", bounds->width, bounds->height);
//		cvSetImageROI(frame, *bounds);
//		IplImage *writeImage = cvCreateImage(cvSize(100,100), frame->depth, frame->nChannels);
//		cvResize(frame, writeImage, CV_INTER_LINEAR);
//		sprintf(faceImageName, "faces/face%d.ppm", faceCount++);
//		cvSaveImage(faceImageName, writeImage, 0);
//		cvReleaseImage(&writeImage);
	}
	else
	{
	  printf("no face.\n");
		*center_x = -1;
		*center_y = -1;
		*width = 0;
		*height = 0;
	}
	cvReleaseImage(&frame);
	return;
}
