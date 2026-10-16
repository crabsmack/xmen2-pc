/* SentinelsA1_FrameHook - the Coolant Core's scene hook (g_SceneFrameHook), once per frame (twice
 * in two-player step mode): count the frame, count the zap lights' timer down to 0, step the
 * palette cycles (unless a fade runs) and the zap lights. */
#include "xmen2.h"

#define g_ZapTimer	(*(s32 *)g_SceneVar3B18)	/* scene variable: frames to the zap lights' next step */

void SentinelsA1_FrameHook(void)
{
	register long n = g_TwoPlayerStep ? 2 : 1;

	do {
		++g_FrameCounter;
		if (g_ZapTimer > 0)
			--g_ZapTimer;
		if (!g_PalFadeActive)
			SentinelsA1_CyclePalettes();
		SentinelsA1_StepZapLights();
	} while (--n);
}
