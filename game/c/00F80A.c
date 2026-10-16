/* AvalonB1_CamHookWait60 - AvalonB1 intro, camera hook 2: the players held still with no input
 * for 60 frames, then hook 3, AvalonB1_CamHookScroll120. The camera stays where it is.
 *
 * cam: the camera (unused). */
#include "xmen2.h"

#define g_AvalonB1Timer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (AvalonB1_FrameHook counts) */

void AvalonB1_CamHookWait60(Camera *cam)
{
	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
	if (g_AvalonB1Timer >= 60) {
		g_AvalonB1Timer = 0;
		g_Camera->update = AvalonB1_CamHookScroll120;
	}
	if (IS_ALIVE(&g_Player1))
		STOP_MOVING(&g_Player1);
	if (IS_ALIVE(&g_Player2))
		STOP_MOVING(&g_Player2);
	AvalonB1_ScrollCameraTo(g_CameraX, g_CameraY);
}
