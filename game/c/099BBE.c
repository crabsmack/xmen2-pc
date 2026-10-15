/* XClone_CamSetArenaMode - camera post-update hook while a clone arena is locked: dead zones $A0
 * wide and $70 high on either side of the centre, tracking by XClone_CamArenaUpdate.
 *
 * cam: the camera (g_Camera). */
#include "xmen2.h"

void XClone_CamSetArenaMode(Camera *cam)
{
	cam->deadLeft = 0xA0;
	cam->deadRight = 0xA0;
	cam->deadTop = 0x70;
	cam->deadBottom = 0x70;
	cam->track = XClone_CamArenaUpdate;
}
