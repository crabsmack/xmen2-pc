/* Pal_FadeOutWait - fade the screen to black and wait until it is black (Teleporter_StateTeleport).
 *
 * delay: frames per fade step.
 * Saves the base palette and the palette-cycle script first (Pal_FadeInWait restores both),
 * and leaves the base palette black. */
#include "xmen2.h"

void Pal_FadeOutWait(long delay)
{
	g_PalBaseDirty = 0;
	Pal_SaveBase();
	Pal_SaveCycle();
	Pal_StartFadeDelay(7, delay, -1, CODEPTR(Tbl_FadeBlack));
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	Pal_ClearBase();
}
