/* AvalonB1_PalFadeStep - one step of the AvalonB1 fade-in (AvalonB1_CamHookFadeIn): palette line 0
 * rebuilt from the colours saved by AvalonB1_InitCamera through one row of a level table.
 *
 * row:    the step (row of levels).
 * levels: Tbl_AvalonB1FadeInLevels, 8 bytes per row: the level shown for each 3-bit colour
 *         component value (a negative first byte ends the table).
 * Each component (red bits 1-3, green 5-7, blue 9-11) of saved colour i becomes
 * levels[row][component]. Colours 6-8 never get darker than they are now; colours 9
 * and 15 are left alone. Uploaded at the next V-blank. */
#include "xmen2.h"

#define g_AvalonB1SavedPal	(*(PTR(s16) *)g_SceneVar3B14)	/* scene variable: line 0 saved by AvalonB1_InitCamera (a buffer in AvalonB1_RunScene's frame) */

/* A 3-bit component of a colour word: red (shift 1), green (5), blue (9). */
#define COMP(c, s)	((unsigned short)((c) >> (s) & 7))

void AvalonB1_PalFadeStep(short row, s8 *levels)
{
	register s16 *src;
	register s16 *p;
	register char r, g, b;
	register short i;

	g_PalBaseDirty = 1;
	row <<= 3;
	src = g_AvalonB1SavedPal;
	p = g_PalBase;
	for (i = 0; i < 16; i++, src++, p++) {
		r = *(levels + row + COMP(*src, 1));
		g = *(levels + row + COMP(*src, 5));
		b = *(levels + row + COMP(*src, 9));
		switch (i) {
		case 6:
		case 7:
		case 8:
			if (r < COMP(*p, 1))
				r = COMP(*p, 1);
			if (g < COMP(*p, 5))
				g = COMP(*p, 5);
			if (b < COMP(*p, 9))
				b = COMP(*p, 9);
		default:
			g_PalBase[i] = (r << 1) + (g << 5) + (b << 9);
			break;
		case 9:
		case 15:
			break;
		}
	}
}
