/* SentinelsB1_FrameHook - the girder stage's scene hook (g_SceneFrameHook), once per frame: the
 * camera's dead zone ends 200 px down the screen (Camera.deadBottom), count the frame, step the
 * Sentinels warning light (unless a fade runs); in two-player step mode the count and the light
 * twice. Returns nothing (no caller reads d0). */
#include "xmen2.h"

void SentinelsB1_FrameHook(void)
{
	g_Camera->deadBottom = 200;
	g_FrameCounter++;
	if (!g_PalFadeActive)
		SentinelsD1_AnimLight();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			SentinelsD1_AnimLight();
	}
}
