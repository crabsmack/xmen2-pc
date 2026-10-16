/* SavageJungle_PalPhaseUpdate - the jungle's palette phases, every frame: when the phase timer
 * has run out, the next phase (g_JunglePalPhase) starts: phases 1-5 each set palette line 2's
 * colours 2-5 (the light darkening towards evening) and take their length from the phase words
 * (g_JunglePalScript[3..7], seconds); the timer becomes that x 60 frames (a later phase keeps
 * the timer at 0: 0 frames) and the palette is uploaded at the next V-blank unless a fade runs.
 * The timer then counts down by 1 (so after the last phase it runs from -1 on).
 *
 * The cases are the disassembly's fragments SavageJungle_PalPhaseUpdate_Case0 (phase 1), _Case1
 * (2, 3, 4, 5 follow it) and _Done (the common tail). No phase words (g_JunglePalScript 0) would
 * read address 6 on; the one scene that runs this (SavageB1_FrameHook) has them. */
#include "xmen2.h"

void SavageJungle_PalPhaseUpdate(void)
{
	if (!g_JunglePalTimer) {
		++g_JunglePalPhase;
		switch (g_JunglePalPhase) {
		case 1:		/* SavageJungle_PalPhaseUpdate_Case0 */
			g_JunglePalTimer = g_JunglePalScript[3];
			g_PalBase[0x22] = 0x200;
			g_PalBase[0x23] = 0x400;
			g_PalBase[0x24] = 0x600;
			g_PalBase[0x25] = 0x802;
			break;
		case 2:		/* SavageJungle_PalPhaseUpdate_Case1 */
			g_JunglePalTimer = g_JunglePalScript[4];
			g_PalBase[0x22] = 0x200;
			g_PalBase[0x23] = 0x400;
			g_PalBase[0x24] = 0x602;
			g_PalBase[0x25] = 0x804;
			break;
		case 3:
			g_JunglePalTimer = g_JunglePalScript[5];
			g_PalBase[0x22] = 0x200;
			g_PalBase[0x23] = 0x402;
			g_PalBase[0x24] = 0x604;
			g_PalBase[0x25] = 0x806;
			break;
		case 4:
			g_JunglePalTimer = g_JunglePalScript[6];
			g_PalBase[0x22] = 0x402;
			g_PalBase[0x23] = 0x404;
			g_PalBase[0x24] = 0x606;
			g_PalBase[0x25] = 0x808;
			break;
		case 5:
			g_JunglePalTimer = g_JunglePalScript[7];
			g_PalBase[0x22] = 0x404;
			g_PalBase[0x23] = 0x426;
			g_PalBase[0x24] = 0x608;
			g_PalBase[0x25] = 0x82A;
		}
		/* SavageJungle_PalPhaseUpdate_Done */
		g_JunglePalTimer = g_JunglePalTimer * 60;
		g_PalBaseDirty = !g_PalFadeActive;
	}
	--g_JunglePalTimer;
}
