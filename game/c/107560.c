/* SpaceB1_FrameHook - the Maintenance Shaft's scene hook (g_SceneFrameHook), once per frame: count
 * the frame and step the palette cycles (SpaceB1_CyclePalettes, unless a fade runs); in two-player
 * step mode both twice. Returns nothing (no caller reads d0). */
#include "xmen2.h"

void SpaceB1_FrameHook(void)
{
	g_FrameCounter++;
	if (!g_PalFadeActive)
		SpaceB1_CyclePalettes();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive)
			SpaceB1_CyclePalettes();
	}
}
