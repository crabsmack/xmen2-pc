/* CloneA1_FrameHook - the CloneA1 scene hook, once per frame: count the frame and cycle the
 * palettes (CloneA1_CyclePalettes, not while a fade runs); twice in two-player step mode. */
#include "xmen2.h"

void CloneA1_FrameHook(void)
{
	g_FrameCounter++;
	if (!g_PalFadeActive)
		CloneA1_CyclePalettes();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			CloneA1_CyclePalettes();
	}
}
