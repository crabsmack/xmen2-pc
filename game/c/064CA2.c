/* Cerebro_AnimatePalettes - the Cerebro screens' palette animation, once per frame
 * (Cerebro_FrameHook), paused while a palette fade runs. Two sequences:
 * - every 4th frame a glow in palette line 3, colours $C-$F, steps back and forth through 8
 *   stages (direction +1 at stage 0, -1 at stage 7; cases Cerebro_AnimatePalettes_CasesA), the
 *   palette marked for upload (Cerebro_AnimatePalettes_StepB);
 * - every 5th frame line 0 colour 7 and colour $F step through an 8-stage cycle
 *   (Cerebro_AnimatePalettes_CasesB, Cerebro_AnimatePalettes_EndB). */
#include "xmen2.h"

#define g_GlowTimer	(*(s16 *)g_SceneVar3B38)	/* scene variable: frames to the glow's next stage */
#define g_GlowStage	(*(s16 *)g_SceneVar3B40)	/* scene variable: the glow's stage 0-7 */
#define g_GlowDir	(*(s16 *)g_SceneVar3B48)	/* scene variable: the glow's direction (+1, -1) */
#define g_CycleTimer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames since the cycle's last step */
#define g_CycleStage	(*(s16 *)g_SceneVar3B3A)	/* scene variable: the cycle's stage 0-7 */

void Cerebro_AnimatePalettes(void)
{
	if (!g_PalFadeActive) {
		--g_GlowTimer;
		if (g_GlowTimer < 0) {
			g_GlowTimer = 3;
			g_GlowStage += g_GlowDir;
			switch (g_GlowStage) {
			case 0:
				g_PalBase[0x3B] = 0x60;
				g_PalBase[0x3A] = 0x40;
				g_PalBase[0x39] = 6;
				g_PalBase[0x38] = 4;
				g_GlowDir = 1;
				break;
			case 1:
				g_PalBase[0x3B] = 0x60;
				g_PalBase[0x3A] = 0x40;
				g_PalBase[0x39] = 6;
				g_PalBase[0x38] = 4;
				break;
			case 2:
				g_PalBase[0x3B] = 0x80;
				g_PalBase[0x3A] = 0x60;
				g_PalBase[0x39] = 8;
				g_PalBase[0x38] = 6;
				break;
			case 3:
				g_PalBase[0x3B] = 0x80;
				g_PalBase[0x3A] = 0x80;
				g_PalBase[0x39] = 8;
				g_PalBase[0x38] = 8;
				break;
			case 4:
				g_PalBase[0x3B] = 0xA0;
				g_PalBase[0x3A] = 0x80;
				g_PalBase[0x39] = 0xA;
				g_PalBase[0x38] = 8;
				break;
			case 5:
				g_PalBase[0x3B] = 0xA0;
				g_PalBase[0x3A] = 0xA0;
				g_PalBase[0x39] = 0xA;
				g_PalBase[0x38] = 0xA;
				break;
			case 6:
				g_PalBase[0x3B] = 0xC0;
				g_PalBase[0x3A] = 0xA0;
				g_PalBase[0x39] = 0xC;
				g_PalBase[0x38] = 0xA;
				break;
			case 7:
				g_PalBase[0x3B] = 0xC0;
				g_PalBase[0x3A] = 0xC0;
				g_PalBase[0x39] = 0xC;
				g_PalBase[0x38] = 0xC;
				g_GlowDir = -1;
				break;
			}
			g_PalBaseDirty = 1;
		}
		++g_CycleTimer;
		if (g_CycleTimer > 3) {
			g_CycleTimer = 0;
			switch (g_CycleStage) {
			case 0:
				g_PalBase[7] = 0xA20;
				g_PalBase[0xF] = 0x62C;
				break;
			case 1:
				g_PalBase[7] = 0xA20;
				g_PalBase[0xF] = 0x62A;
				break;
			case 2:
				g_PalBase[7] = 0x820;
				g_PalBase[0xF] = 0x628;
				break;
			case 3:
				g_PalBase[7] = 0x820;
				g_PalBase[0xF] = 0x62A;
				break;
			case 4:
				g_PalBase[7] = 0x620;
				g_PalBase[0xF] = 0x62C;
				break;
			case 5:
				g_PalBase[7] = 0x620;
				g_PalBase[0xF] = 0x62A;
				break;
			case 6:
				g_PalBase[7] = 0x820;
				g_PalBase[0xF] = 0x628;
				break;
			case 7:
				g_PalBase[7] = 0x820;
				g_PalBase[0xF] = 0x62A;
				break;
			}
			++g_CycleStage;
			if (g_CycleStage > 7)
				g_CycleStage = 0;
			g_PalBaseDirty = 1;
		}
	}
}
