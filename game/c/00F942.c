/* AvalonB1_CamHookFadeIn - AvalonB1 intro, camera hook 4: the players held still with no input
 * while the camera goes on scrolling up 2 px a frame (4 with g_TwoPlayerStep); every 6 frames
 * one step of the fade-in (AvalonB1_PalFadeStep, the next row of Tbl_AvalonB1FadeInLevels).
 * At the table's end (a negative byte) the HUD comes on and hook 5, AvalonB1_CamHookFinish.
 *
 * cam: the camera (unused). */
#include "xmen2.h"

#define g_AvalonB1Timer		(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (AvalonB1_FrameHook counts) */
#define g_AvalonB1FadeRow	(*(s16 *)g_SceneVar3B3A)	/* scene variable: fade-in step (row of Tbl_AvalonB1FadeInLevels) */

void AvalonB1_CamHookFadeIn(Camera *cam)
{
	register s8 *levels;
	register short x, y;

	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
	if (g_AvalonB1Timer >= 6) {
		g_AvalonB1Timer = 0;
		levels = (s8 *)CODEPTR(Tbl_AvalonB1FadeInLevels);
		if (levels[g_AvalonB1FadeRow << 3] >= 0) {
			AvalonB1_PalFadeStep(g_AvalonB1FadeRow, levels);
			g_AvalonB1FadeRow++;
		} else {
			g_AvalonB1Timer = 0;
			g_Camera->update = AvalonB1_CamHookFinish;
			g_HudOn = 1;
		}
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
