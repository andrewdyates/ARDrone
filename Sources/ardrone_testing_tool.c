/**
 * @file main.c
 * @author sylvain.gaeremynck@parrot.com
 * @date 2009/07/01
 */
#include <ardrone_testing_tool.h>

//ARDroneLib
#include <ardrone_tool/ardrone_time.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <ardrone_tool/Control/ardrone_control.h>
#include <ardrone_tool/UI/ardrone_input.h>

//Common
#include <config.h>
int GetClassifier(CvHaarClassifierCascade *classifier);
int GetPredefinedCvMemory(CvMemStorage *storage);
#include <ardrone_api.h>

//VP_SDK
#include <ATcodec/ATcodec_api.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Api/vp_api_thread_helper.h>
#include <VP_Os/vp_os_signal.h>

//Local project
#include <UI/gamepad.h>
#include <Video/video_stage.h>

#include "UI/gui.h"
#include "cv.h"
#include "highgui.h"

const char* FRONT_CASCADE_FILE = "front_cascade.xml";
CvHaarClassifierCascade *frontCascade = NULL;
CvMemStorage *frontStorage = NULL;
int initialized = 0;

static int32_t exit_ihm_program = 1;


int GetCvClassifier(CvHaarClassifierCascade **classifier)
{
  if(initialized == 0) {
    frontCascade = (CvHaarClassifierCascade*)cvLoad(FRONT_CASCADE_FILE, 0, 0, 0);
    if (frontCascade != NULL && frontStorage != NULL) {
      initialized = 1;
    }
  }
  *classifier = frontCascade;
  return (frontCascade == NULL)?(1):(0);
}

int GetCvStorage(CvMemStorage **storage)
{
  if(initialized == 0) {
    frontStorage = cvCreateMemStorage(0);
    if (frontCascade != NULL && frontStorage != NULL) {
      initialized = 1;
    }
  }
  *storage = frontStorage;
  return (frontStorage == NULL)?(1):(0);
}

DEFINE_THREAD_ROUTINE(gui, data) {
  gdk_threads_enter();
  gtk_main();
  gdk_threads_leave();
}

/* Implementing Custom methods for the main function of an ARDrone application */

/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(int argc, char **argv)
{

  /* Register GUI */
  init_gui(argc, argv); /* Creating the GUI */
  START_THREAD(gui, NULL); /* Starting the GUI thread */

  /* Registering for a new device of game controller */
  ardrone_tool_input_add( &gamepad );

  /* Start all threads of your application */
  START_THREAD( video_stage, NULL );
  
  return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom()
{

  /* Relinquish all threads of your application */
  JOIN_THREAD( video_stage );

  JOIN_THREAD(gui);

  /* Unregistering for the current device */
  ardrone_tool_input_remove( &gamepad );

  return C_OK;
}

/* The event loop calls this method for the exit condition */
bool_t ardrone_tool_exit()
{
  return exit_ihm_program == 0;
}

C_RESULT signal_exit()
{
  exit_ihm_program = 0;

  return C_OK;
}

/* Implementing thread table in which you add routines of your application and those provided by the SDK */
BEGIN_THREAD_TABLE
  THREAD_TABLE_ENTRY( gui, 20)
// we also aren't controlling the drone in this thread
  THREAD_TABLE_ENTRY( ardrone_control, 20 )
// we don't care about navdata right now
//  THREAD_TABLE_ENTRY( navdata_update, 20 )
  THREAD_TABLE_ENTRY( video_stage, 20 )
END_THREAD_TABLE

