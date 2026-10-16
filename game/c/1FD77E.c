/* Pal_ApplyFade - build the palette shown during a fade (g_PalWork) from the base palette: each
 * colour component (Genesis colour ----BBB-GGG-RRR-) goes through the fade table at this level.
 *
 * level: fade level (0-7).
 * table: the fade table: 8 rows of 8 signed bytes, one row per component value, one byte per
 *        level (Tbl_FadeBlack...).
 * With g_PalFadeLine -1 all 64 colours are faded, else that palette line's 16 only (the others
 * are copied unchanged). */
#include "xmen2.h"

/* The faded value of component c (0-7) at the table's level. */
#define FADE(f, c)	(*((f) + (((c) & 7) << 3)))

void Pal_ApplyFade(long level, void *table)
{
	register s8 *f;
	short i;
	short c;

	f = (s8 *)table;
	f += level;
	if (g_PalFadeLine == -1) {
		for (i = 0; i < 64; i++) {
			c = g_PalBase[i];
			g_PalWork[i] = (FADE(f, c >> 9) << 9) + (FADE(f, c >> 5) << 5) + (FADE(f, c >> 1) << 1);
		}
	} else {
		for (i = 0; i < 64; i++)
			g_PalWork[i] = g_PalBase[i];
		for (i = g_PalFadeLine << 4; i < (g_PalFadeLine << 4) + 16; i++) {
			c = g_PalBase[i];
			g_PalWork[i] = (FADE(f, c >> 9) << 9) + (FADE(f, c >> 5) << 5) + (FADE(f, c >> 1) << 1);
		}
	}
}
