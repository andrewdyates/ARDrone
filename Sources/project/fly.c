/* Flight commands given a frame's centroid and mass. */

#include "ppm.h"
#include <math.h>

#include <ardrone_tool/UI/ardrone_input.h>


int fly(int x, int y, int mass, int debug) {
  /* Command drone to fly. */
  int xoff, yoff, radius;
  float phi, theta, gaz, yaw;
  float xrange = WIDTH/2.0;
  float yrange = HEIGHT/2.0;

  // if no sufficent mass detected. hover and exit
  if (mass <= 8) {
    ardrone_at_set_progress_cmd(0,0.0,0.0,0.0,0.0);
    if (debug) {
      printf("No mass. Hovering.\n");
    }
    return 1;
  }
  
  radius = round(sqrt(mass / 3.1415));
  xoff = x-(int)xrange;
  yoff = y-(int)yrange;

  // no angular tilt for now
  phi = 0.0;         // left/right angle
  theta = 0.0;       // front/back angle

  // Simple +/-1.0 view range for turns
  // do not (yet) account for current flight angle
  gaz = yoff/yrange * 1.2; // vertical speed
  yaw = xoff/xrange * 1.2; // angular speed
  if (gaz > 1.0) gaz = 1.0;
  if (gaz < -1.0) gaz = -1.0;
  if (yaw > 1.0) yaw = 1.0;
  if (yaw < -1.0) yaw = -1.0;
  
  // first bit: 0=hover mode; 1=dynamic turns
  ardrone_at_set_progress_cmd(0, phi, theta, gaz, yaw);

  
  if (debug) {
    printf("phi=%1.2f, theta=%1.2f, gaz=%1.2f, yaw=%1.2f\n", phi, theta, gaz, yaw);
  }
  return 0;
}


