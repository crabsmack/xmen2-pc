/* AvalonB1_TileHook - AvalonB1 map tile hook (g_TileHooks[1]): map blocks $1F and $20 at a cell
 * become block $1E (Map_SetBlockFG).
 *
 * col, row: the map cell (FG block map).
 * Returns 1. */
#include "xmen2.h"

long AvalonB1_TileHook(register short col, register short row)
{
	register short block;

	block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (block == 0x1F || block == 0x20)
		Map_SetBlockFG(col, row, 0x1E, 0);
	return 1;
}
