/* CloneA2_BgPalTimer - a timer of the CloneA2 scene (every frame): palette line 2 follows the
 * camera's height, one of four background palettes per 16 px band of g_CameraY (bands cycle
 * Pal_CloneA2Bg0-3); a new band copies its palette in and marks the palette dirty. Nothing while
 * the scene's palette work is off or a fade runs.
 *
 * t: its timer (unused). Started by CloneA2_StartBgPalTimer.
 * The fade flag is tested twice. The switch cases are the fragments CloneA2_BgPalTimer_Case0
 * (all four) and CloneA2_BgPalTimer_Copy (the copy). */
#include "xmen2.h"

#define g_BgPalBand	(*(s16 *)g_SceneVar3B02)	/* scene variable: background palette shown, 0-3 */
#define g_CloneA2PalOff	(*(s16 *)g_SceneVar3B04)	/* scene variable: non-zero: no flicker, no background palette swaps */

void CloneA2_BgPalTimer(Timer *t)
{
	register s16 *pal;
	register short prev;

	if (!g_CloneA2PalOff && !g_PalFadeActive && !g_PalFadeActive) {
		prev = g_BgPalBand;
		g_BgPalBand = g_CameraY >> 4 & 3;
		if (prev != g_BgPalBand) {
			switch (g_BgPalBand) {
			case 0: pal = (s16 *)CODEPTR(Pal_CloneA2Bg0); break;	/* CloneA2_BgPalTimer_Case0 */
			case 1: pal = (s16 *)CODEPTR(Pal_CloneA2Bg1); break;
			case 2: pal = (s16 *)CODEPTR(Pal_CloneA2Bg2); break;
			case 3: pal = (s16 *)CODEPTR(Pal_CloneA2Bg3); break;
			}
			{	/* CloneA2_BgPalTimer_Copy */
				register s16 *src = pal;
				register s16 *dst = &g_PalBase[0x20];
				register long i;

				for (i = 0; i < 16; i++)
					*dst++ = *src++;
			}
			g_PalBaseDirty = 1;
		}
	}
}
