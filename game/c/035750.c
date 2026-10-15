/* AvalonA4_FrameHook - the Throne Room's per-frame hook (g_SceneFrameHook): count the frame and
 * step the palette animation (AvalonA4_AnimatePalettes) unless a palette fade runs; in
 * two-player mode (g_TwoPlayerStep) all of it twice. */
#include "xmen2.h"

void AvalonA4_FrameHook(void)
{
	g_FrameCounter++;
	if (!g_PalFadeActive)
		AvalonA4_AnimatePalettes();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			AvalonA4_AnimatePalettes();
	}
}
