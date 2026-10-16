/* Pal_FadeInWait - restore the palette Pal_FadeOutWait saved and fade it in from black, waiting
 * until it is done, then restart the saved palette-cycle script (Teleporter_StateArrive).
 *
 * delay: frames per fade step. */
#include "xmen2.h"

void Pal_FadeInWait(long delay)
{
	g_PalBaseDirty = 0;
	Pal_RestoreBase();
	Pal_StartFadeDelay(0, delay, -1, CODEPTR(Tbl_FadeBlack));
	while (g_PalFadeActive)
		VDP_WaitVBlank();
	Pal_RestoreCycle();
}
