/* SavageA3_FrameHook - the Perimeter's per-frame hook (g_SceneFrameHook): count the frame, step
 * the SavageA2 light animation (SavageA2_AnimLights), and the empty SavageA3_FrameStub unless a
 * palette fade runs; all of it twice in two-player step mode (g_TwoPlayerStep). */
#include "xmen2.h"

void SavageA3_FrameHook(void)
{
	g_FrameCounter++;
	SavageA2_AnimLights();
	if (!g_PalFadeActive)
		SavageA3_FrameStub();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		SavageA2_AnimLights();
		if (!g_PalFadeActive)
			SavageA3_FrameStub();
	}
}
