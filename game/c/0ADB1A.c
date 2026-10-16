/* TrapDoor_SetTiles - TRAPDOOR class slot +$82: draw the door with one map block across its
 * segment, in the foreground map (Map_SetBlockFG).
 *
 * self: the object.  tile: the map block (tileClosed ... tileWide).
 * Columns from (x1 + 8) >> 5 to (x2 - 8) >> 5 of its segment (32-pixel blocks), the row of the
 * object's y (y >> 13: pixels >> 5). */
#include "xmen2.h"

void TrapDoor_SetTiles(TrapDoor *self, short tile)
{
	short first = (self->door->x1 + 8) >> 5;
	short last = (self->door->x2 - 8) >> 5;
	short row = self->y >> 13;
	long col;

	for (col = first; col <= last; col++)
		Map_SetBlockFG(col, row, tile, 0);	/* (a long column, pushed as is) */
}
