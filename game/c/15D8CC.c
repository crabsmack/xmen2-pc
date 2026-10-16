/* Map_SetBlockBG - put another block in BG map cell (col, row) and redraw it (SiberiaB1_IceTileHit).
 *
 * block:     the new block number (bits 0-8).
 * keepHooks: 0: clear the cell's hook type (bits $3800).
 * The cell's other bits are kept. */
#include "xmen2.h"

void Map_SetBlockBG(register short col, register short row, short block, char keepHooks)
{
	register s16 *cell;
	register short v;

	cell = &g_BgBlockMap[g_BgRowTable[row] + col];
	v = (*cell & ~0x1FF) | block;
	if (!keepHooks)
		v &= ~0x3800;
	*cell = v;
	Map_RedrawBlock(col, row, 1);
}
