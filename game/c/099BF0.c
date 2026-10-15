/* XClone_CamSetNormal - camera back to following the player normally (class slot +$1E,
 * FollowPlayer: dead zones $60, $5A, $E0, $C0), and note the camera mode switch.
 *
 * cam: the camera (g_Camera; XClone_UnlockArena, BroodClaw_Swing). */
#include "xmen2.h"

void XClone_CamSetNormal(Camera *cam)
{
	g_CamModeSwitched = 1;
	(*cam->cls->method[6])(cam, 0x60, 0x5A, 0xE0, 0xC0);	/* FollowPlayer */
}
