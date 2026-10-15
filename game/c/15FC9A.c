/* Bitmap_FromTileWords - build a bitmap of the tile indexes a list of tile words uses.
 *
 * map: the bitmap.  words: the tile words.  count: how many (low word).  mask: the index bits.
 * Clears count / 16 + 1 words of the bitmap, then sets bit (word & mask) for every word with a
 * non-zero index. Hand-written 68000; Level_LoadUsedTiles calls it. */
#include "xmen2.h"

void Bitmap_FromTileWords(void *map, void *words, long count, short mask)
{
	s16 *clear = (s16 *)map;
	u8 *bits = (u8 *)map;
	s16 *w = (s16 *)words;
	unsigned short k;
	unsigned short tile;

	for (k = ((unsigned short)count >> 4) + 1; k; k--)
		*clear++ = 0;
	for (k = (unsigned short)count; k; k--) {
		tile = *w++ & mask;
		if (tile != 0)
			bits[tile >> 3] |= 1 << (tile & 7);
	}
}
