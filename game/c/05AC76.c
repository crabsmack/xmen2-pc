/* BaniMazaB2_CamHook - the BaniMazaB2 (Exterior) camera's post-update hook (Cam_New's hook): the
 * dead zone's bottom is $D2 below the screen top.
 *
 * cam: the camera. */
#include "xmen2.h"

void BaniMazaB2_CamHook(Camera *cam)
{
	cam->deadBottom = 0xD2;
}
