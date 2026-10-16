/* Cerebro_IntroPalHook - the V-blank game hook of the Cerebro intro (g_VBlankGameHook,
 * Cerebro_PlayIntro): the screen's palette animation (Cerebro_AnimatePalettes), then the intro's
 * frame countdown (g_SceneVar3B0C, from 10000) steps the fades: from $C9 down to $A9 palette line
 * 2 takes stage (($C9 - count) / 4) of Pal_CerebroIntroFadeLine2 (16 colours each), from $2C down
 * to $14 line 1 stage ($2C - count) / 4 of Pal_CerebroIntroFadeLine1; each marked for upload. */
#include "xmen2.h"

#define g_CerebroIntro		(*(s16 *)g_SceneVar3B0C)	/* scene variable: the intro's frame countdown */

void Cerebro_IntroPalHook(void)
{
	register s16 *a;
	register s16 *b;

	Cerebro_AnimatePalettes();
	--g_CerebroIntro;
	if (g_CerebroIntro <= 0xC9 && g_CerebroIntro >= 0xA9) {
		register short step;
		register long i;

		a = (s16 *)CODEPTR(Pal_CerebroIntroFadeLine2);
		step = (0xC9 - g_CerebroIntro) >> 2;
		b = &a[step];
		a = &g_PalBase[0x20];
		for (i = 0; i < 16; i++)
			*a++ = *b++;
		g_PalBaseDirty = 1;
	}
	if (g_CerebroIntro <= 0x2C && g_CerebroIntro >= 0x14) {
		register short step;
		register long i;

		b = (s16 *)CODEPTR(Pal_CerebroIntroFadeLine1);
		step = ((0x2C - g_CerebroIntro) & 0xFFC) << 2;
		a = &b[step];
		b = &g_PalBase[0x10];
		for (i = 0; i < 16; i++)
			*b++ = *a++;
		g_PalBaseDirty = 1;
	}
}
