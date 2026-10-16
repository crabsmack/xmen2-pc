/* AvalonA3_LoadLevel - load the Exodus arena (AvalonA3): its level data through Level_Load (FG
 * map Lvl_AvalonA3FgMap, FG tiles Lvl_AvalonA4Fg, BG map and tiles Lvl_AvalonA2Bg, FG floor
 * segments Lvl_AvalonA3FgSegs, no BG segments, flags 0 0 1 1, class-init argument $A, music $44,
 * grid shifts 8 and 8, class list Tbl_AvalonA3ClassList), then music 7 (restarted), the HUD on,
 * background scroll factors $7F (y) and $DC (x), one-player stepping, palette line 2 from
 * Pal_AvalonA2Line2, colour 0 black, and the camera's lowest target 32 px higher.
 * MPW: the BG map and tiles go through the palette copy's two pointer variables (the ROM loads
 * them into a4 and a3 before the call). */
#include "xmen2.h"

void AvalonA3_LoadLevel(void)
{
	register s16 *src;
	register s16 *dst;
	register long i;

	dst = (s16 *)CODEPTR(Lvl_AvalonA2Bg);	/* BG map */
	src = (s16 *)CODEPTR(Lvl_AvalonA2Bg);	/* BG tiles */
	Level_Load(CODEPTR(Lvl_AvalonA3FgMap), CODEPTR(Lvl_AvalonA4Fg), dst, src,
		CODEPTR(Lvl_AvalonA3FgSegs), 0, 0, 0, 1, 1, 0xA, 0x44, 8, 8, 0, CODEPTR(Tbl_AvalonA3ClassList));
	Snd_Stop(7);
	Snd_Play(7);
	g_HudOn = 1;
	g_BgScrollFactorY = 0x7F;
	g_BgScrollFactorX = 0xDC;
	g_TwoPlayerStep = 0;
	src = (s16 *)CODEPTR(Pal_AvalonA2Line2);
	dst = &g_PalBase[32];
	for (i = 0; i < 16; i++)
		*dst++ = *src++;
	g_PalBase[0] = 0;
	g_CamMaxYTarget -= 0x20;
}
