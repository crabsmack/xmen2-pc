/* SavageA2_FrameHook - SavageA2's per-frame hook (g_SceneFrameHook): count the frame and step the
 * warning lights (SavageA2_AnimLights); twice in two-player step mode (g_TwoPlayerStep). */
#include "xmen2.h"

void SavageA2_FrameHook(void)
{
	g_FrameCounter++;
	SavageA2_AnimLights();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		SavageA2_AnimLights();
	}
}
