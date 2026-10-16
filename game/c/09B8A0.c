/* BroodClaw_CamApplyShake - camera apply function (+$0C) BroodClaw_Swing installs when the boss
 * scroll ends: play one step of the camera-shake script g_CamShake (as Cam_ShakeStep) and set the
 * scroll to the camera position plus its offsets, the look offset (y) and the step's shake.
 *
 * cam: the camera.
 * Script bytes: 'd' ($64) ends it (g_CamShake = 0); 'e' ($65) n jumps back n bytes; 'f' ($66) x
 * calls Sys_Nop1F945C(x, 0); anything else is a signed (dx, dy) pair, for this step only. */
#include "xmen2.h"

/* the script as signed bytes (the global is an untyped pointer: ROM tables set it) */
#define g_Shake	(*(PTR(s8) *)&g_CamShake)

void BroodClaw_CamApplyShake(Camera *cam)
{
	short dx;
	short dy;

	dx = 0;
	dy = 0;
	if (g_Shake) {
		if (*g_Shake == 'd')
			g_Shake = 0;
		else if (*g_Shake == 'e')
			g_Shake -= g_Shake[1];
		else if (*g_Shake == 'f') {
			g_Shake++;
			Sys_Nop1F945C(*g_Shake++, 0);
		} else {
			dx = *g_Shake++;
			dy = *g_Shake++;
		}
	}
	Cam_SetPos(cam->x + cam->offsetX + dx, cam->y + cam->offsetY + cam->look + dy);
}
