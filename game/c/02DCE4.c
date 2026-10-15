/* Exodus_BreakFloorTiles - draw a broken floor section: 4 map blocks in row $18 (columns 2-5 on
 * the left, 6-9 on the right), each redrawn (Map_RefreshBlock), with sound $E2.
 *
 * left: nonzero for the left section (segment A), 0 for the right (segment B).
 * The blocks are $27, the scene's extra block g_FgBlockCount (+2 on the right), the next one,
 * and $28. */
#include "xmen2.h"

void Exodus_BreakFloorTiles(char left)
{
	register short row = 0x18;
	register short col;
	register short block;
	register s16 *p;

	Snd_Stop(0xE2);
	Snd_Play(0xE2);
	if (left) {
		col = 2;
		block = g_FgBlockCount;
	} else {
		col = 6;
		block = g_FgBlockCount + 2;
	}
	p = &g_FgBlockMap[g_FgRowTable[row] + col];
	*p = 0x27;
	Map_RefreshBlock(col, row);
	++col;
	p = &g_FgBlockMap[g_FgRowTable[row] + col];
	*p = block;
	Map_RefreshBlock(col, row);
	++col;
	p = &g_FgBlockMap[g_FgRowTable[row] + col];
	*p = block + 1;
	Map_RefreshBlock(col, row);
	++col;
	p = &g_FgBlockMap[g_FgRowTable[row] + col];
	*p = 0x28;
	Map_RefreshBlock(col, row);
}
