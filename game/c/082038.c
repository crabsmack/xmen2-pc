/* CloneA2_CamAltTimer - the timer that ends the CloneA2 camera's alternate mode (every frame):
 * once g_FloatFallSpeed (here a frame countdown set to $300 by CloneA2_StartCamAltMode) is 0,
 * the camera goes back to CloneA2_CamHookNormal with its axes free (CAMERA Cam_UnlockAxes),
 * g_CamModeSwitched is set and the timer frees itself.
 *
 * t: its timer. */
#include "xmen2.h"

void CloneA2_CamAltTimer(Timer *t)
{
	if (!g_FloatFallSpeed) {
		CloneA2_CamHookNormal(g_Camera);
		(*g_Camera->cls->method[13])(g_Camera);		/* +$3A Cam_UnlockAxes */
		g_CamModeSwitched = 1;
		Timer_Free(t);
	}
}
