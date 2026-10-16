/* Map_SetBlockFG - put another block in FG map cell (col, row) and redraw it (scene tile hooks:
 * broken walls, opened doors).
 *
 * block:     the new block number (bits 0-8).
 * keepHooks: 0: clear the cell's hook type (bits $3800), so it stops calling its tile hook.
 * The cell's other bits (flips, priority, solid) are kept. */
#include "xmen2.h"

void Map_SetBlockFG(register short col, register short row, short block, char keepHooks)
{
	register s16 *cell;
	register short v;

	cell = &g_FgBlockMap[g_FgRowTable[row] + col];
	v = (*cell & ~0x1FF) | block;
	if (!keepHooks)
		v &= ~0x3800;
	*cell = v;
	Map_RedrawBlock(col, row, 0);
}
