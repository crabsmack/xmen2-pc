/* SentinelsB1_LoadLevel - load the girder stage (SentinelsB1 Exterior_3): its level data through
 * Level_Load (FG map and tiles Lvl_SentinelsB1Fg, BG map and tiles Lvl_SentinelsB1Bg, FG floor
 * segments Lvl_SentinelsB1FgSegs, no BG segments, flags 0 0 0 1, class-init argument $10, music 4,
 * grid shifts 8 and 8, class list Tbl_SentinelsB1ClassList), then palette colour 0 black, palette line 2
 * from Pal_SentinelsLine2 (16 colours), the HUD on, background scroll factors $C0 (y) and $80 (x),
 * one-player stepping. */
#include "xmen2.h"

void SentinelsB1_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Level_Load(CODEPTR(Lvl_SentinelsB1Fg), CODEPTR(Lvl_SentinelsB1Fg), CODEPTR(Lvl_SentinelsB1Bg),
		CODEPTR(Lvl_SentinelsB1Bg), CODEPTR(Lvl_SentinelsB1FgSegs), 0, 0, 0, 0, 1, 0x10, 4, 8, 8, 0,
		CODEPTR(Tbl_SentinelsB1ClassList));
	g_PalBase[0] = 0;
	src = (s16 *)CODEPTR(Pal_SentinelsLine2);
	dst = &g_PalBase[0x20];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_HudOn = 1;
	g_BgScrollFactorY = 0xC0;
	g_BgScrollFactorX = 0x80;
	g_TwoPlayerStep = 0;
}
