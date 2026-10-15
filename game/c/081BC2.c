/* CloneA2_FlickerInit - reset the CloneA2 palette flicker (CloneA2_FlickerUpdate): colours 13/14
 * step every 2 frames from the end of the 26-step pattern, colour 15 every 9 frames from case
 * 6; the flicker runs (g_CloneA2PalOff 0). */
#include "xmen2.h"

#define g_FlickerTimer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames to the next colour 13/14 step */
#define g_FlickerIndex	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step in Tbl_CloneA2FlickerPattern (counts down from $1A) */
#define g_Color15Timer	(*(s16 *)g_SceneVar3B34)	/* scene variable: frames to the next colour 15 step */
#define g_Color15Index	(*(s16 *)g_SceneVar3B3C)	/* scene variable: colour 15 step (counts down from 6) */
#define g_CloneA2PalOff	(*(s16 *)g_SceneVar3B04)	/* scene variable: non-zero: no flicker, no background palette swaps */

void CloneA2_FlickerInit(void)
{
	g_FlickerTimer = 2;
	g_FlickerIndex = 0x1A;
	g_Color15Timer = 9;
	g_Color15Index = 6;
	g_CloneA2PalOff = 0;
}
