/* AvalonB1_CamHookScroll120 - AvalonB1 intro, camera hook 3: the players held still with no input
 * while the camera scrolls up 2 px a frame (4 with g_TwoPlayerStep); after 120 frames plane B's
 * scroll y back to 0 and hook 4, AvalonB1_CamHookFadeIn.
 *
 * cam: the camera (unused). */
#include "xmen2.h"

#define g_AvalonB1Timer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (AvalonB1_FrameHook counts) */

void AvalonB1_CamHookScroll120(Camera *cam)
{
	register short x, y;

	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
	if (g_AvalonB1Timer >= 120) {
		g_AvalonB1Timer = 0;
		g_BgScrollY = 0;
		g_Camera->update = AvalonB1_CamHookFadeIn;
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
	x = g_CameraX;
	y = g_CameraY - (char)(g_TwoPlayerStep ? 4 : 2);
	AvalonB1_ScrollCameraTo(x, y);
}
