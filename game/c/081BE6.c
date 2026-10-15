/* CloneA2_FlickerUpdate - one frame of the CloneA2 palette flicker (palette line 0): every 2
 * frames the next step of Tbl_CloneA2FlickerPattern (backwards, 26 steps) sets colours 13/14
 * (step value 1: $068/$8CC, else $468/$8EE); every 9 frames colour 15 steps through $80, $A0,
 * $C0, $A0, $80, $A0 (counting down from case 5). Each change marks the palette dirty.
 *
 * Run by CloneA2_FrameHook and XClone_FrameHook (the clone maker's room uses it too).
 * The colour-15 switch cases are the fragments CloneA2_FlickerUpdate_Color15Case0 (all six) and
 * CloneA2_FlickerUpdate_SetColor15 (the store); a step outside 0-5 would store whatever d7
 * held. */
#include "xmen2.h"

#define g_FlickerTimer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames to the next colour 13/14 step */
#define g_FlickerIndex	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step in Tbl_CloneA2FlickerPattern (counts down from $1A) */
#define g_Color15Timer	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames to the next colour 15 step */
#define g_Color15Index	(*(s16 *)g_SceneVar3B3C)	/* scene variable: colour 15 step (counts down from 6) */

void CloneA2_FlickerUpdate(void)
{
	register u8 *pattern = (u8 *)Tbl_CloneA2FlickerPattern;
	register char step;
	register long colour;

	if (!g_FlickerTimer) {
		g_FlickerTimer = 2;
		if (!g_FlickerIndex)
			g_FlickerIndex = 0x1A;
		--g_FlickerIndex;
		step = pattern[g_FlickerIndex] - 1;
		if (step) {
			g_PalBase[13] = 0x468;
			g_PalBase[14] = 0x8EE;
		} else {
			g_PalBase[13] = 0x068;
			g_PalBase[14] = 0x8CC;
		}
		g_PalBaseDirty = 1;
	}
	--g_FlickerTimer;
	if (!g_Color15Timer) {
		g_Color15Timer = 9;
		if (!g_Color15Index)
			g_Color15Index = 6;
		--g_Color15Index;
		switch (g_Color15Index) {
		case 0: colour = 0xA0; break;	/* CloneA2_FlickerUpdate_Color15Case0 */
		case 1: colour = 0x80; break;
		case 2: colour = 0xA0; break;
		case 3: colour = 0xC0; break;
		case 4: colour = 0xA0; break;
		case 5: colour = 0x80; break;
		}
		g_PalBase[15] = colour;		/* CloneA2_FlickerUpdate_SetColor15 */
		g_PalBaseDirty = 1;
	}
	--g_Color15Timer;
}
