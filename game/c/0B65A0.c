/* SavageA4_CamSetHooks - the Chamber camera's post-update hook (Cam_New's hook, +$1C): the dead
 * zone from screen x $A0 to $A0 (none: x follows exactly) and its tracking function
 * SavageA4_CamUpdate.
 *
 * cam: the camera. */
#include "xmen2.h"

void SavageA4_CamSetHooks(Camera *cam)
{
	cam->deadLeft = 0xA0;
	cam->deadRight = 0xA0;
	cam->track = SavageA4_CamUpdate;
}
