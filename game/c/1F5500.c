/* Map_SetRowPriority - give the 16 tile words (4 x 4 tiles) of one block definition the priority
 * bit ($8000), so the block is drawn in front of sprites.
 *
 * defs:  the block definitions (g_FgBlockDefs: 16 words each).
 * count: the level's block count (g_FgBlockCount).
 * n:     which block after them: block count + n - 1 (scene blocks appended to the level's). */
#include "xmen2.h"

void Map_SetRowPriority(void *defs, short count, short n)
{
	u16 *p;
	short i;

	p = (u16 *)defs + ((count + n - 1) << 4);
	for (i = 0; i < 16; i++)
		*p++ |= 0x8000;
}
