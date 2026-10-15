/* MagnetoBoss_CrackWall - crack the wall blocks of a map column one stage ($8C -> $A9 -> $AA ->
 * $AB -> $AC, broken), or straight to broken; sound $63.
 *
 * col:     map column (blocks).  row0, row1: first and last row.
 * instant: break at once.
 * Returns 1 when it broke the wall (a row went to $AC), else 0. A block that is none of the
 * stages gets the previous row's new block (the first row: whatever the register held; the
 * walls it is called for start with a wall block). The empty if (block) is a dead test the
 * ROM has. */
#include "xmen2.h"

char MagnetoBoss_CrackWall(short col, short row0, short row1, char instant)
{
	char broken = 0;
	unsigned long block;
	register long row;
	register long tile;

	for (row = row0; row <= row1; row++) {
		block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
		if (block) {
		}
		if (instant) {
			tile = 0xAC;
			broken = 1;
		} else if (block == 0x8C)
			tile = 0xA9;
		else if (block == 0xA9)
			tile = 0xAA;
		else if (block == 0xAA)
			tile = 0xAB;
		else if (block == 0xAB) {
			tile = 0xAC;
			broken = 1;
		}
		Map_SetBlockFG(col, row, tile, 0);
	}
	Snd_Stop(0x63);
	Snd_Play(0x63);
	return broken;
}
