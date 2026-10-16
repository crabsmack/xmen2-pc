/* AvalonA2_ToggleTileHook - the Falling Room's map tile hook (g_TileHooks[2]): a player touched a
 * floor trigger block: start the retracting floor nearest to that row
 * (RetractFloor_TriggerNearest), toggle the block's bit 9, clear its palette bits ($3800) and
 * redraw it (Map_RefreshBlock).
 *
 * col, row: the map cell (FG block map).
 * The cell's centre in world coordinates (8 fraction bits) is computed for x, then for y, and
 * never used (the read in the if (0) keeps MPW C from dropping the variable, as the ROM still
 * computes both). Returns 0. */
#include "xmen2.h"

long AvalonA2_ToggleTileHook(register short col, register short row)
{
	register s16 *cell = &g_FgBlockMap[g_FgRowTable[row] + col];
	register long pos;

	pos = (col << 5) + 16 << 8;
	pos = (row << 5) + 16 << 8;
	RetractFloor_TriggerNearest(row);
	*cell ^= 0x200;
	g_FgBlockMap[g_FgRowTable[row] + col] &= ~0x3800;
	Map_RefreshBlock(col, row);
	if (0)
		(void)pos;
	return 0;
}
