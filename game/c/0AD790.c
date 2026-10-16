/* SavageA2_CamSetHooks - SavageA2 camera's post-update hook (Cam_New's hook, +$1C): the flood
 * camera's update (SavageA2_CamUpdateFlood) and scroll (SavageA2_CamApply), the dead zone from
 * screen y 100 to $A0.
 *
 * cam: the camera. */
#include "xmen2.h"

void SavageA2_CamSetHooks(Camera *cam)
{
	cam->update = SavageA2_CamUpdateFlood;
	cam->apply = SavageA2_CamApply;
	cam->deadTop = 100;
	cam->deadBottom = 0xA0;
}
