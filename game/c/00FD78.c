/* AvalonB1_FrameHook - AvalonB1's per-frame scene hook (g_SceneFrameHook): g_FrameCounter and the
 * intro stage timer advance (by 2 with g_TwoPlayerStep), then the palette cycles
 * (AvalonB1_CyclePalettes). */
#include "xmen2.h"

#define g_AvalonB1Timer	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames in this intro stage (the camera hooks reset it) */

void AvalonB1_FrameHook(void)
{
	g_FrameCounter++;
	g_AvalonB1Timer++;
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		g_AvalonB1Timer++;
	}
	AvalonB1_CyclePalettes();
}
