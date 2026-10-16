/* Pal_Update - the palette's part of each V-blank (VDP_FrameUpdate): during a fade, every
 * g_PalFadeDelay frames upload the faded palette and move one level on (up when fading in,
 * down when fading out), ending past level 7 or below 0; otherwise upload the working or the
 * base palette if it was changed. */
#include "xmen2.h"

void Pal_Update(void)
{
	if (g_PalFadeActive) {
		g_PalFadeTimer--;
		if (g_PalFadeTimer <= 0) {
			g_PalFadeTimer = g_PalFadeDelay;
			VDP_DmaPalette(g_PalWork);
			if (g_PalFadeUp)
				g_PalFadeLevel++;
			else
				g_PalFadeLevel--;
			if ((g_PalFadeUp & (g_PalFadeLevel > 7)) || (!g_PalFadeUp & (g_PalFadeLevel < 0)))
				g_PalFadeActive = 0;
			else
				Pal_ApplyFade(g_PalFadeLevel, g_PalFadeTable);
		}
	} else if (g_PalWorkDirty) {
		g_PalWorkDirty = 0;
		VDP_DmaPalette(g_PalWork);
	} else if (g_PalBaseDirty) {
		g_PalBaseDirty = 0;
		VDP_DmaPalette(g_PalBase);
	}
}
