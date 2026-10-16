/* AvalonB1_CamHookFinish - AvalonB1 intro, camera hook 5: the camera scrolls up 2 px a frame (4
 * with g_TwoPlayerStep, the players held still with no input) until its y is $60 or less; then,
 * once neither live player has flags bit $1000, the level starts: sound on, sound 0 (the
 * music, restarted), the parallax Tbl_AvalonB1Parallax, palette line 0 restored
 * (AvalonB1_RestorePalette) and the camera's normal update (Cam_DefaultUpdate). The input is
 * cleared on every call.
 *
 * cam: the camera (its x/y follow the scroll). */
#include "xmen2.h"

void AvalonB1_CamHookFinish(register Camera *cam)
{
	register char ready = 1;
	register short x, y;

	g_Pad1Pressed = 0;
	g_Pad2Pressed = 0;
	g_Pad1Latch = 0;
	g_Pad2Latch = 0;
	if (g_CameraY <= 0x60) {
		if (IS_ALIVE(&g_Player1) && (g_Player1.flags & 0x1000))
			ready = 0;
		if (IS_ALIVE(&g_Player2) && (g_Player2.flags & 0x1000))
			ready = 0;
		if (ready) {
			g_SndMute = 0;
			Snd_Stop(0);
			Snd_Play(0);
			Scroll_SetParallax(CODEPTR(Tbl_AvalonB1Parallax));
			AvalonB1_RestorePalette();
			g_Camera->update = Cam_DefaultUpdate;
		}
	} else {
		if (IS_ALIVE(&g_Player1))
			STOP_MOVING(&g_Player1);
		if (IS_ALIVE(&g_Player2))
			STOP_MOVING(&g_Player2);
		cam->x = g_CameraX;
		x = g_CameraX;
		y = cam->y = g_CameraY - (char)(g_TwoPlayerStep ? 4 : 2);
		AvalonB1_ScrollCameraTo(x, y);
	}
}
