/* Tile_RemapWords - replace the index part of tile words through a lookup table.
 *
 * words: the tile words.  table: new index per old index (words).  mask: the index bits.
 * count: how many words (low word).
 * Each word becomes (word & ~mask) | table[word & mask]. Hand-written 68000; Level_LoadUsedTiles
 * calls it to compact the tiles a level uses. */
#include "xmen2.h"

void Tile_RemapWords(void *words, void *table, short mask, long count)
{
	s16 *w = (s16 *)words;
	s16 *map = (s16 *)table;
	unsigned short k;

	for (k = (unsigned short)count; k; k--) {
		*w = (*w & ~mask) | map[*w & mask];
		w++;
	}
}
