/* BaniMazaA1_SwitchTileHook - map tile hook 1 of the Moving Blocks: a switch block touched at
 * (col, row) flips (tile bit 9, redrawn: Map_RefreshBlock), the rolling-disc band at its height
 * reverses (RollingDiscs_ReverseAtY), sounds $DE and $E7 (3 stopped), then 40 frames before a
 * switch works again (g_BmA1SwitchTimer).
 *
 * col, row: the map block (blocks). Returns 0. */
#include "xmen2.h"

long BaniMazaA1_SwitchTileHook(register short col, register short row)
{
	register s16 *blk;

	if (!g_BmA1SwitchTimer) {
		blk = &g_FgBlockMap[g_FgRowTable[row] + col];
		*blk ^= 0x200;
		Map_RefreshBlock(col, row);
		RollingDiscs_ReverseAtY(row << 5);
		Snd_Stop(0xDE);
		Snd_Play(0xDE);
		Snd_Stop(3);
		Snd_Stop(0xE7);
		Snd_Play(0xE7);
		g_BmA1SwitchTimer = 0x28;
	}
	return 0;
}
