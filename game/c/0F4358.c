/* SentinelsB1_LoadLevel - load the girder stage (SentinelsB1 Exterior_3): its level data through
 * Level_Load (FG map and tiles Art_SentinelsB1Nem2, BG map and tiles Art_SentinelsB1Nem, FG floor
 * segments Lvl_SentinelsB1Segs, no BG segments, flags 0 0 0 1, class-init argument $10, music 4,
 * grid shifts 8 and 8, class list Art_SentinelsB1Nem3), then palette colour 0 black, palette line 2
 * from Pal_SentinelsLine2 (16 colours), the HUD on, background scroll factors $C0 (y) and $80 (x),
 * one-player stepping. */
#include "xmen2.h"

void SentinelsB1_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Level_Load(CODEPTR(Art_SentinelsB1Nem2), CODEPTR(Art_SentinelsB1Nem2), CODEPTR(Art_SentinelsB1Nem),
		CODEPTR(Art_SentinelsB1Nem), CODEPTR(Lvl_SentinelsB1Segs), 0, 0, 0, 0, 1, 0x10, 4, 8, 8, 0,
		CODEPTR(Art_SentinelsB1Nem3));
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
