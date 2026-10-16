/* BaniMazaA1_CamHook - the Moving Blocks camera's post-update hook (Cam_New's hook, +$1C): the
 * dead zone's bottom at screen y $A0.
 *
 * cam: the camera. */
#include "xmen2.h"

void BaniMazaA1_CamHook(Camera *cam)
{
	cam->deadBottom = 0xA0;
}
