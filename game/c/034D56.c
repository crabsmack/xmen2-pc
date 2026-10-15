/* AvalonA4_ResetEffectCounters - restart AvalonA4's palette animation (AvalonA4_AnimatePalettes):
 * pulse table step 0, first sequence in 5 frames at step 0, second in 4 frames at step 0. */
#include "xmen2.h"

#define g_A4PulseDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames before the next pulse tick */
#define g_A4PulseStep	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step of the 6-step pulse sequence */
#define g_A4GlowDelay	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames before the next glow step */
#define g_A4GlowStep	(*(s16 *)g_SceneVar3B3C)	/* scene variable: step of the 15-step glow sequence */

void AvalonA4_ResetEffectCounters(void)
{
	AvalonA4_NopStatic();
	g_AvalonA4PalStep = 0;
	g_A4PulseDelay = 5;
	g_A4PulseStep = 0;
	g_A4GlowDelay = 4;
	g_A4GlowStep = 0;
}
