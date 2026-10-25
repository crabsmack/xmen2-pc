/* AvalonB1_CamHookWait2 - AvalonB1 intro, camera hook 1 (AvalonB1_InitCamera): the players held
 * still with no input; after 2 frames the intro palettes (AvalonB1_InitIntroPalette) and hook 2,
 * AvalonB1_CamHookWait60. The camera stays where it is (redrawn there).
 *
 * cam: the camera (unused). */
#include "xmen2.h"

#define g_AvalonB1Timer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (AvalonB1_FrameHook counts) */

void AvalonB1_CamHookWait2(Camera *cam)
{
	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
	if (g_AvalonB1Timer >= 2) {
		g_AvalonB1Timer = 0;
		g_Camera->update = AvalonB1_CamHookWait60;
		AvalonB1_InitIntroPalette();
	}
	if (IS_ALIVE(&g_Player1))
		STOP_MOVING(&g_Player1);
	if (IS_ALIVE(&g_Player2))
		STOP_MOVING(&g_Player2);
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		g_Pad2Pressed = 0;	/* its pad, no input (its context) */
		g_Pad2Latch = 0;
		if (IS_ALIVE(q))
			STOP_MOVING(q);
	});
#endif
	AvalonB1_ScrollCameraTo(g_CameraX, g_CameraY);
}
