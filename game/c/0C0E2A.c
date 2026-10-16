/* SavageC1_FrameHook - Jungle 1's scene hook (g_SceneFrameHook), once per frame: count the frame
 * (twice in two-player step mode), then refresh the music's player parameters
 * (Music_SetPlayerParams) on 31 of every 32 frames (not when the count is a multiple of 32).
 * Returns nothing (no caller reads d0). */
#include "xmen2.h"

void SavageC1_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	if (g_FrameCounter & 0x1F)
		Music_SetPlayerParams();
}
