/* Map_SetBlockFGFlip - put another block, with its own flips, in FG map cell (col, row) and
 * redraw it.
 *
 * block:     the new block number (bits 0-8).
 * keepHooks: 0: clear the cell's hook type (bits $3800).
 * hflip, vflip: flip the block (bits $200, $400; the cell's old flips are dropped).
 * The cell's priority and solid bits are kept. */
#include "xmen2.h"

void Map_SetBlockFGFlip(register short col, register short row, short block, char keepHooks,
	char hflip, char vflip)
{
	register s16 *cell;
	register short v;

	cell = &g_FgBlockMap[g_FgRowTable[row] + col];
	v = block | (*cell & 0xF800);
	if (!keepHooks)
		v &= ~0x3800;
	if (hflip)
		v |= 0x200;
	if (vflip)
		v |= 0x400;
	*cell = v;
	Map_RedrawBlock(col, row, 0);
}
