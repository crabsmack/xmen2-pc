/* AvalonA4_CamHook - the Throne Room camera's post-update hook (Cam_New's hook, +$1C): the
 * scroll's top limit is y $20 (g_CamMinY and its target), and the camera looks up only until
 * the lowest player is $D8 below the screen top, with the dead zone's bottom there too.
 *
 * cam: the camera. */
#include "xmen2.h"

void AvalonA4_CamHook(register Camera *cam)
{
	register long top = 0x20;

	g_CamMinYTarget = (short)top;
	g_CamMinY = (short)top;
	cam->lookMaxScrY = 0xD8;
	cam->deadBottom = 0xD8;
}
