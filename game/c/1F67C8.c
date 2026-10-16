/* Pal_StartFadeDelay - start a palette fade, like Pal_StartFade, with its frames per step.
 *
 * level: start level (0: fade in, upward; else fade out from there, downward).
 * delay: frames per fade step.
 * line:  the palette line to fade, or -1 for all four.
 * table: the fade table (Tbl_FadeBlack...).
 * Applies the first step at once (Pal_ApplyFade); Pal_Update steps the rest. */
#include "xmen2.h"

void Pal_StartFadeDelay(register short level, long delay, short line, void *table)
{
	g_PalFadeTimer = 0;
	g_PalFadeDelay = (short)delay;
	g_PalFadeLevel = level;
	g_PalFadeUp = level == 0;
	g_PalFadeLine = line;
	g_PalFadeTable = table;
	Pal_ApplyFade(g_PalFadeLevel, g_PalFadeTable);
	g_PalFadeActive = 1;
}
