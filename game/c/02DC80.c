/* Exodus_StartPsiFlash - start EXODUS's screen-wide psychic flash: 14 palette steps
 * (g_PsiFlashTimer, run by AvalonA3_PsiFlashStep) and sound $E1.
 *
 * Called when a horizontal blast reaches a wall (ExodusBlast_StateFlyRight/Left). */
#include "xmen2.h"

void Exodus_StartPsiFlash(void)
{
	g_PsiFlashTimer = 14;
	Snd_Stop(0xE1);
	Snd_Play(0xE1);
}
