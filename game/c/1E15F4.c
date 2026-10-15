/* Cyclops_ResetBeamFlicker - clear the state of the beam's V-blank flicker
 * (Cyclops_BeamVBlankFlicker): nothing pending, off, no splice (new and current).
 *
 * Called by Cyclops_ResetBeams at the end of a scene. Hand-written 68000 (no MPW frame, tag
 * asm: one word and two long clears): this C is the same behaviour. */
#include "xmen2.h"

void Cyclops_ResetBeamFlicker(void)
{
	short i;

	g_BeamFlickerPending = 0;
	g_BeamFlickerOn = 0;
	for (i = 0; i < 4; i++) {
		g_BeamFlickerCur[i] = 0;
		g_BeamFlickerNew[i] = 0;
	}
}
