/* Seg_FindInBlock - the floor segment of a type that contains the centre of a 32-px map block.
 *
 * type: segment type; col, row: the block. Seg_FindContaining at the block's centre
 * (col * 32 + 16, row * 32 + 16). Returns the segment, or 0. */
#include "xmen2.h"

Seg *Seg_FindInBlock(short type, short col, short row)
{
	short x = (col << 5) + 16;
	short y = (row << 5) + 16;

	return Seg_FindContaining(x, y, type);
}
