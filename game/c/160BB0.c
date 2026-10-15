/* Cam_ShakeStep - camera step (shake): play one step of the camera-shake script g_CamShake.
 *
 * self: the camera. Script bytes: 'd' ($64) ends it (g_CamShake = 0); 'e' ($65) n jumps back n
 * bytes (a loop); 'f' ($66) x calls Sys_Nop1F945C(x, 0); anything else is a signed (dx, dy)
 * pair added to the camera position. Nothing without a script. */
#include "xmen2.h"

/* the script as signed bytes (the global is an untyped pointer: ROM tables set it) */
#define g_Shake	(*(PTR(s8) *)&g_CamShake)

void Cam_ShakeStep(Camera *self)
{
	register Camera *c = self;

	if (!g_Shake)
		return;
	if (*g_Shake == 'd')
		g_Shake = 0;
	else if (*g_Shake == 'e')
		g_Shake -= g_Shake[1];
	else if (*g_Shake == 'f') {
		g_Shake++;
		Sys_Nop1F945C(*g_Shake++, 0);
	} else {
		c->x += *g_Shake++;
		c->y += *g_Shake++;
	}
}
