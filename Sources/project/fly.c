/* Flight commands given a frame's centroid and mass. */

#include "ppm.h"
#include <math.h>

#include <ardrone_tool/UI/ardrone_input.h>

#define MIN_MASS 6


int fly(int x, int y, int mass, int mass_thresh) {
  /* Command drone to fly to follow a target.
     
  Args:
    x: x coordinate of target centroid
    y: y coordinate of target centroid
    mass: (rough) area of target (typically number of pixels or box area)
    mass_thresh: mass at which the drone neither flys forward nor backward
  Returns:
    1 if hover, 0 if movement
   */
  int xoff, yoff, radius;
  float phi, theta, gaz, yaw;
  int move_flag; // 0 hover mode, 1 hover off
  float xrange, yrange;
  float fmass, fmass_thresh;

  // if no sufficent mass detected, then hover and exit
  if (mass <= MIN_MASS) {
    ardrone_at_set_progress_cmd(0,0.0,0.0,0.0,0.0);
    printf("No mass. Hovering.\n");
    return 1;
  }
  // reset the "watchdog" to activate gaz movement
  ardrone_at_reset_com_watchdog();
  

  // COMPUTE FLIGHT PARAMTERS
  // ========================
  // 1. Angular tilt (phi)
  // ----------------------
  // Never tilt.
  phi = 0.0;         // left/right angle

  /* // 2. Forward tilt (theta) */
  /* // ---------------------- */
  /* // area linearly scales with square root */
  /* fmass = sqrt((float)mass); */
  /* fmass_thresh = sqrt((float)mass_thresh); */
  /* theta = (mass_thresh - fmass) / mass_thresh; */
  /* // scale theta */
  /* theta *= 0.5; */
  /* // cap theta at maximum amplitude */
  /* if (theta > 1.0) theta = 1.0; */
  /* if (theta < -1.0) theta = -1.0; */
  /* // if abs(theta)is small, activate hover mode. */
  /* if (theta > -0.05 && theta < 0.05) { */
  /*   move_flag = 0; // hover in place (only turn and rise/fall) */
  /* } else { */
  /*   move_flag = 1; // activate motion */
  /* } */
  // do not move for now
  theta = 0.0;
  move_flag = 0;

  // 3. Stationary Movement (gaz (y) and yaw (x))
  // ----------------------    
  // x and y offsets from center of image
  // do not (yet) correct for current flight angle theta
  xrange = WIDTH/2.0;
  yrange = HEIGHT/2.0;
  xoff = x-(int)xrange;
  yoff = y-(int)yrange;
  // gaz and yaw +/- 1.0 scaled ratio distance from center
  // do not dampen for mass (yet)
  gaz = yoff/yrange * 0.3; // vertical speed
  yaw = xoff/xrange * 0.6;  // angular speed
  // correct out-of-bounds
  if (gaz > 1.0) gaz = 1.0;
  if (gaz < -1.0) gaz = -1.0;
  if (yaw > 1.0) yaw = 1.0;
  if (yaw < -1.0) yaw = -1.0;

  // 4. Send flight command
  // first bit: 0=hover mode; second bit: 1=dynamic turns
  ardrone_at_set_progress_cmd(move_flag, phi, theta, -gaz, yaw);

  // print status to console
  printf("phi=%1.2f, theta=%1.2f, gaz=%1.2f, yaw=%1.2f\n", phi, theta, -gaz, yaw);
  printf("radius (from mass): %d\n", radius);

  return 0;
}
