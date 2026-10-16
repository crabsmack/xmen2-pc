/* SentinelsD1_LoadLevel - load the Sentinels base exterior (SentinelsD1): its level data through
 * Level_Load (FG map and tiles Art_Nem_0D3972, BG map and tiles Art_SentinelsB1Nem, FG floor
 * segments Lvl_Segs_0D32C2, no BG segments, flags 0 0 0 1, class-init argument $10, music 4, grid
 * shifts 8 and 8, class list Art_Nem_0DA5A8), then palette colour 0 black, palette line 2 from
 * Pal_SentinelsLine2 (16 colours), the HUD on, background scroll factors $C0 (y) and $80 (x),
 * one-player stepping.
 * Returns nothing (no caller reads d0). */
#include "xmen2.h"

void SentinelsD1_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	Level_Load(CODEPTR(Art_Nem_0D3972), CODEPTR(Art_Nem_0D3972), CODEPTR(Art_SentinelsB1Nem),
		CODEPTR(Art_SentinelsB1Nem), CODEPTR(Lvl_Segs_0D32C2), 0, 0, 0, 0, 1, 0x10, 4, 8, 8, 0,
		CODEPTR(Art_Nem_0DA5A8));
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
