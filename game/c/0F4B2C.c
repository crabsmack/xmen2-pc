/* SentinelsB2_LoadLevel - load the Sentinels escape (SentinelsB2): its level data through
 * Level_Load (FG map and tiles Lvl_SentinelsB1Fg, BG map and tiles Lvl_SentinelsB1Bg, FG floor
 * segments Lvl_SentinelsB2FgSegs, no BG segments, flags 0 0 0 1, class-init argument $10, no music
 * (-1), grid shifts 8 and 8, spawn from the BG map, class list Tbl_SentinelsB2ClassList), the stubs;
 * the escape music $60 (every sound off first) only when progress flag $8000 is already set,
 * then that flag set; palette lines 0 and 2 at the alarm's normal colours (colour 0 black too),
 * background scroll factors $C0 (y) and $80 (x), one-player stepping. */
#include "xmen2.h"

void SentinelsB2_LoadLevel(void)
{
	Level_Load(CODEPTR(Lvl_SentinelsB1Fg), CODEPTR(Lvl_SentinelsB1Fg), CODEPTR(Lvl_SentinelsB1Bg),
		CODEPTR(Lvl_SentinelsB1Bg), CODEPTR(Lvl_SentinelsB2FgSegs), 0, 0, 0, 0, 1, 0x10, -1, 8, 8, 1,
		CODEPTR(Tbl_SentinelsB2ClassList));
	SentinelsB2_CallStubs();
	if (g_ProgressFlags & 0x8000) {
		Snd_StopAll();
		Snd_Stop(0x60);
		Snd_Play(0x60);
	}
	g_ProgressFlags |= 0x8000;
	g_PalBase[0] = 0;
	g_PalBase[1] = 0;
	g_PalBase[2] = 2;
	g_PalBase[3] = 4;
	g_PalBase[4] = 6;
	g_PalBase[5] = 8;
	g_PalBase[6] = 0xA;
	g_PalBase[7] = 0;
	g_PalBase[8] = 2;
	g_PalBase[9] = 4;
	g_PalBase[0xA] = 6;
	g_PalBase[0xC] = 0x8E;
	g_PalBase[0xD] = 0x4A;
	g_PalBase[0xE] = 0x6E;
	g_PalBase[0xF] = 0x2E;
	g_PalBase[0x21] = 0;
	g_PalBase[0x22] = 2;
	g_PalBase[0x23] = 4;
	g_PalBase[0x24] = 6;
	g_PalBase[0x25] = 8;
	g_PalBase[0x26] = 0xA;
	g_PalBase[0x27] = 0xA;
	g_PalBase[0x28] = 0xC;
	g_PalBase[0x29] = 0xE;
	g_PalBase[0x2A] = 6;
	g_PalBase[0x2B] = 8;
	g_PalBase[0x2C] = 0xA;
	g_PalBase[0x2D] = 0xC;
	g_PalBase[0x2E] = 4;
	g_PalBase[0x2F] = 0xE;
	g_BgScrollFactorY = 0xC0;
	g_BgScrollFactorX = 0x80;
	g_TwoPlayerStep = 0;
}
