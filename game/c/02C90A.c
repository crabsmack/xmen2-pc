/* AvalonA3_PsiFlashStep - one frame of EXODUS's screen-wide psychic flash (g_PsiFlashTimer, 14
 * frames from Exodus_StartPsiFlash): while it runs, both players are hurt (Exodus_HurtPlayer)
 * and colours 0-13 of palette line 0 rotate down by one; on its last frame lines 0 and 2 get
 * back the colours AvalonA3_SavePalettes kept. The palette is uploaded at the next V-blank.
 * MPW: not matched: the ROM also saves d6 (never used; its rotation value is in d5). The
 * block around the two calls is where the ROM pops their arguments. Verified by shadow tests. */
#include "xmen2.h"

#define g_A3SavedPal0	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: colours 0-13 of line 0 before the flash */
#define g_A3SavedPal2	(*(PTR(s16) *)g_SceneVar3B18)	/* scene variable: colours 0-13 of line 2 */

void AvalonA3_PsiFlashStep(void)
{
	register long i;
	register long first;

	g_PsiFlashTimer--;
	g_PalBaseDirty = 1;
	if (g_PsiFlashTimer) {
		if (1) {
			Exodus_HurtPlayer(&g_Player1);
			Exodus_HurtPlayer(&g_Player2);
		}
		first = g_PalBase[0];
		for (i = 0; i <= 12; i++)
			g_PalBase[i] = g_PalBase[i + 1];
		g_PalBase[13] = first;
	} else {
		for (i = 0; i <= 13; i++) {
			g_PalBase[i] = g_A3SavedPal0[i];
			g_PalBase[32 + i] = g_A3SavedPal2[i];
		}
	}
}
