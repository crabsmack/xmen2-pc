/* AvalonB2_TileHook - the Inner Shell's map tile hook (g_TileHooks[1]): map blocks $1A and $1B at
 * a cell become block $19 (Map_SetBlockFG).
 *
 * col, row: the map cell (FG block map).
 * Returns 1. */
#include "xmen2.h"

long AvalonB2_TileHook(register short col, register short row)
{
	register short block;

	block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (block == 0x1A || block == 0x1B)
		Map_SetBlockFG(col, row, 0x19, 0);
	return 1;
}
