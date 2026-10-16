/* Level_CompactUsedItems - renumber the used entries of a list (bit set in the bitmap) 0, 1, 2...,
 * optionally moving their data down to match.
 *
 * bits: the used-entry bitmap (Bitmap_Test).  count: entries.  remap: gets each used entry's new
 * number (unused ones keep theirs).  move: also move each used entry's data (1 << shift words
 * at data) down to its new place.  shift: log2 of an entry's words (Level_LoadUsedTiles: 4 for
 * blocks of 16 tile words).
 * Returns the number of used entries. */
#include "xmen2.h"

long Level_CompactUsedItems(void *bits, long count, s16 *remap, char move, s16 *data, register short shift)
{
	register long used = 0;
	register long i;

	for (i = 0; i < count; i++) {
#ifdef XMEN2_MPW
		if (((long (*)())Bitmap_Test)(bits, i)) {	/* called undeclared: tst.l */
#else
		if (Bitmap_Test(bits, (short)i)) {
#endif
			if (move) {
				s16 *d = data;
				register s16 *src = d + (i << shift);
				register s16 *dst = d + (used << shift);
				register long j;

				for (j = 0; j < 1 << shift; j++)
					*dst++ = *src++;
			}
			remap[i] = (short)used;
			used++;
		}
	}
	return used;
}
