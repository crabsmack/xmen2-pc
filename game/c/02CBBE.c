/* AvalonA3_CamHook - the Exodus arena camera's post-update hook (Cam_New's hook, +$1C): the
 * dead zone's bottom at screen y 200.
 *
 * cam: the camera. */
#include "xmen2.h"

void AvalonA3_CamHook(Camera *cam)
{
	cam->deadBottom = 200;
}
