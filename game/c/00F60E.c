/* AvalonB1_RestorePalette - end of the AvalonB1 intro (AvalonB1_CamHookFinish): palette line 0
 * back to the 16 colours AvalonB1_InitCamera saved; uploaded at the next V-blank. */
#include "xmen2.h"

#define g_AvalonB1SavedPal	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: line 0 saved by AvalonB1_InitCamera (a buffer in AvalonB1_RunScene's frame) */

void AvalonB1_RestorePalette(void)
{
	register long i;

	g_PalBaseDirty = 1;
	for (i = 0; i < 16; i++)
		g_PalBase[i] = g_AvalonB1SavedPal[i];
}
