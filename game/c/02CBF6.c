/* AvalonA3_SavePalettes - save colours 0-13 of palette lines 0 and 2 for the psychic flash
 * (AvalonA3_PsiFlashStep restores them when it ends), and no flash running.
 *
 * pal0, pal2: the buffers (14 colours each, in AvalonA3_RunScene's frame), kept in the scene
 * variables g_A3SavedPal0 / g_A3SavedPal2. */
#include "xmen2.h"

#define g_A3SavedPal0	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: colours 0-13 of line 0 before the flash */
#define g_A3SavedPal2	(*(PTR(s16) *)g_SceneVar3B18)	/* scene variable: colours 0-13 of line 2 */

void AvalonA3_SavePalettes(s16 *pal0, s16 *pal2)
{
	register long i;

	g_PsiFlashTimer = 0;
	g_A3SavedPal0 = pal0;
	g_A3SavedPal2 = pal2;
	for (i = 0; i < 14; i++)
		g_A3SavedPal0[i] = g_PalBase[i];
	for (i = 0; i < 14; i++)
		g_A3SavedPal2[i] = g_PalBase[32 + i];
}
