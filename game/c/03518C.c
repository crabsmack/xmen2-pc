/* AvalonA4_SwitchTileHook - map tile hook 1 of the Throne Room: a switch block ($84) touched at
 * (col, row) turns into $89, with sound $1A.
 *
 * col, row: the map block (blocks).
 * Returns 1 (any block). */
#include "xmen2.h"

long AvalonA4_SwitchTileHook(register short col, register short row)
{
	register short tile;

	tile = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
	if (tile == 0x84) {
		Map_SetBlockFG(col, row, 0x89, 0);
		Snd_Stop(0x1A);
		Snd_Play(0x1A);
	}
	return 1;
}
