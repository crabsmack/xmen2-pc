/* SavageA3_TileHookCrack - the Perimeter's map tile hook 2 (g_TileHooks[2]): a breakable block
 * cracks one step further each time it is hit: block $86 -> $8B -> $8C -> $2A (broken; any other
 * block also becomes $2A), its upper bits ($FE00) kept; the broken one is removed
 * (SavageA2_SetBlockTiles, edge block $47).
 *
 * col, row: the map cell (FG block map).
 * Returns what SavageA2_SetBlockTiles returns (0); the hook callers keep it.
 * MPW: the initializers (not assignments) give the ROM's word conversion of the ?: chain. */
#include "xmen2.h"

long SavageA3_TileHookCrack(register short col, short row)
{
	register short block = g_FgBlockMap[g_FgRowTable[row] + col];
	register short tile = block & 0x1FF;
	register short next = tile == 0x86 ? 0x8B : tile == 0x8B ? 0x8C : tile == 0x8C ? 0x2A : 0x2A;

	tile = next | block & ~0x1FF;
	return SavageA2_SetBlockTiles(col, row, tile, next == 0x2A, 0x47);
}
