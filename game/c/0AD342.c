/* SavageA2_TileHookCrack - SavageA2's map tile hook 2 (g_TileHooks[2]): a breakable block cracks
 * one step further each time it is hit: block $83 -> $88 -> $89 -> $27 (broken; any other block
 * also becomes $27), its upper bits ($FE00) kept; the broken one is removed
 * (SavageA2_SetBlockTiles, edge block $AD). SavageA3_TileHookCrack is the same with other tiles.
 *
 * col, row: the map cell (FG block map).
 * Returns what SavageA2_SetBlockTiles returns (0). */
#include "xmen2.h"

long SavageA2_TileHookCrack(register short col, short row)
{
	register short block = g_FgBlockMap[g_FgRowTable[row] + col];
	register short tile = block & 0x1FF;
	register short next = tile == 0x83 ? 0x88 : tile == 0x88 ? 0x89 : tile == 0x89 ? 0x27 : 0x27;

	tile = next | block & ~0x1FF;
	return SavageA2_SetBlockTiles(col, row, tile, next == 0x27, 0xAD);
}
