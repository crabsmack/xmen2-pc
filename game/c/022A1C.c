/* AvalonA2_FrameHook - the Falling Room's per-frame scene hook (g_SceneFrameHook): count the frame
 * and run the (switched-off) palette effect AvalonA2_DisabledFrameEffect unless a palette fade
 * runs; in two-player mode (g_TwoPlayerStep) all of it twice. */
#include "xmen2.h"

void AvalonA2_FrameHook(void)
{
	g_FrameCounter++;
	if (!g_PalFadeActive)
		AvalonA2_DisabledFrameEffect();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			AvalonA2_DisabledFrameEffect();
	}
}
