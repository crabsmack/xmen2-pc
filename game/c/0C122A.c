/* SavageB1_FrameHook - Jungle 2's scene hook (g_SceneFrameHook), once per frame: count the frame
 * (twice in two-player step mode), refresh the music's player parameters (Music_SetPlayerParams)
 * on 31 of every 32 frames, then step the jungle palette phases (SavageJungle_PalPhaseUpdate). */
#include "xmen2.h"

void SavageB1_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	if (g_FrameCounter & 0x1F)
		Music_SetPlayerParams();
	SavageJungle_PalPhaseUpdate();
}
