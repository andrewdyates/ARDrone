#ifndef _MYKONOS_TESTING_TOOL_H_
#define _MYKONOS_TESTING_TOOL_H_

#include <stdio.h>
#include <VP_Os/vp_os_types.h>
#include "cv.h"
#include "highgui.h"

int GetCvClassifier(CvHaarClassifierCascade **classifier);
int GetCvStorage(CvMemStorage **storage);
C_RESULT signal_exit();


#endif // _MYKONOS_TESTING_TOOL_H_
