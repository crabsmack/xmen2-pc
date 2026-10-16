/* AvalonB2_CamHook - the Inner Shell camera's post-update hook (Cam_New's hook, +$1C): looking
 * down stops with the highest live player $60 below the screen top, and the dead zone's bottom
 * is at screen y $A0.
 *
 * cam: the camera. */
#include "xmen2.h"

void AvalonB2_CamHook(register Camera *cam)
{
	cam->lookMinScrY = 0x60;
	cam->deadBottom = 0xA0;
}
