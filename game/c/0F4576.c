/* SentinelsB2_AlarmPalette - the Sentinels escape's alarm flash, one tick per call: while the
 * escape countdown runs (and no fade), a 25-step loop (g_AlarmStep) that sets palette lines 0
 * and 2 to their normal colours at step 0, darker ones at steps 6 and 18, darkest at 12
 * (marking g_PalBaseDirty); the other steps keep the colours. The step tested is the one before
 * this tick's count. */
#include "xmen2.h"

#define g_EscapeTime	(*(s16 *)g_SceneVar3B02)	/* scene variable: escape countdown, tenths of a second */
#define g_AlarmStep	(*(s16 *)g_SceneVar3B38)	/* scene variable: step of the 25-step alarm flash */

void SentinelsB2_AlarmPalette(void)
{
	register long step = g_AlarmStep;

	if (g_PalFadeActive || !g_EscapeTime)
		return;
	++g_AlarmStep;
	if (g_AlarmStep > 24)
		g_AlarmStep = 0;
	if (!step) {
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
		g_PalBaseDirty = 1;
	} else if (step == 6 || step == 18) {
		g_PalBase[1] = 0;
		g_PalBase[2] = 0;
		g_PalBase[3] = 2;
		g_PalBase[4] = 4;
		g_PalBase[5] = 6;
		g_PalBase[6] = 8;
		g_PalBase[7] = 0;
		g_PalBase[8] = 0;
		g_PalBase[9] = 2;
		g_PalBase[0xA] = 4;
		g_PalBase[0xC] = 0x6C;
		g_PalBase[0xD] = 0x48;
		g_PalBase[0xE] = 0x4E;
		g_PalBase[0xF] = 0xC;
		g_PalBase[0x21] = 0;
		g_PalBase[0x22] = 0;
		g_PalBase[0x23] = 2;
		g_PalBase[0x24] = 4;
		g_PalBase[0x25] = 6;
		g_PalBase[0x26] = 8;
		g_PalBase[0x27] = 8;
		g_PalBase[0x28] = 0xA;
		g_PalBase[0x29] = 0xC;
		g_PalBase[0x2A] = 4;
		g_PalBase[0x2B] = 6;
		g_PalBase[0x2C] = 8;
		g_PalBase[0x2D] = 0xA;
		g_PalBase[0x2E] = 2;
		g_PalBase[0x2F] = 0xE;
		g_PalBaseDirty = 1;
	} else if (step == 12) {
		g_PalBase[1] = 0;
		g_PalBase[2] = 0;
		g_PalBase[3] = 0;
		g_PalBase[4] = 2;
		g_PalBase[5] = 4;
		g_PalBase[6] = 6;
		g_PalBase[7] = 0;
		g_PalBase[8] = 0;
		g_PalBase[9] = 0;
		g_PalBase[0xA] = 2;
		g_PalBase[0xC] = 0x4A;
		g_PalBase[0xD] = 6;
		g_PalBase[0xE] = 0x2E;
		g_PalBase[0xF] = 0xA;
		g_PalBase[0x21] = 0;
		g_PalBase[0x22] = 0;
		g_PalBase[0x23] = 0;
		g_PalBase[0x24] = 2;
		g_PalBase[0x25] = 4;
		g_PalBase[0x26] = 6;
		g_PalBase[0x27] = 6;
		g_PalBase[0x28] = 8;
		g_PalBase[0x29] = 0xA;
		g_PalBase[0x2A] = 2;
		g_PalBase[0x2B] = 4;
		g_PalBase[0x2C] = 6;
		g_PalBase[0x2D] = 8;
		g_PalBase[0x2E] = 0;
		g_PalBase[0x2F] = 0xC;
		g_PalBaseDirty = 1;
	}
}
