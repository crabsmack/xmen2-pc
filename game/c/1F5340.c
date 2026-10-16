/* Level_FillBlockSlot - make block n a "slot" block: its 16 tiles are consecutive VRAM tiles of its
 * own, column by column (a sprite-like layout), optionally h-flipped.
 *
 * defs: the block definitions.  base: the slot tiles' first VRAM tile.  n: the block (low 9
 * bits).  first: the first slot block (block first's tiles start at base).  flip: h-flip (tile
 * flag $800 and the columns mirrored).
 * Tile (row, col) of the block = base + ((n - first) & $1FF) * 16 + col * 4 + row. */
#include "xmen2.h"

void Level_FillBlockSlot(void *defs, short base, register short n, short first, register char flip)
{
	register s16 *p = (s16 *)defs + ((n & 0x1FF) << 4);
	register short tile;
	register short row, col;

	tile = base + (((n - first) & 0x1FF) << 4);
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; p++, col++)
			if (flip)
				*p = tile + ((3 - col) << 2) + row + 0x800;
			else
				*p = tile + (col << 2) + row;
}
