/* Pal_StartFade - start a palette fade, 4 frames per step.
 *
 * level: start level (0: fade in, upward; else fade out from there, downward).
 * line:  the palette line to fade, or -1 for all four.
 * table: the fade table (Tbl_FadeBlack, Tbl_FadeInvertWhite...).
 * Applies the first step at once (Pal_ApplyFade); Pal_Update steps the rest. */
#include "xmen2.h"

void Pal_StartFade(register short level, short line, void *table)
{
	g_PalFadeTimer = 0;
	g_PalFadeDelay = 4;
	g_PalFadeLevel = level;
	g_PalFadeUp = 1;
	if (level)
		g_PalFadeUp = 0;
	g_PalFadeLine = line;
	g_PalFadeTable = table;
	Pal_ApplyFade(g_PalFadeLevel, g_PalFadeTable);
	g_PalFadeActive = 1;
}
