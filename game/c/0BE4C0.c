/* Barbarian_StateScrollTiles - BARBARIAN state reached only after 100 taunts (never, see
 * Barbarian_StateTaunt): every 60 frames, in the 8 foreground block rows from the camera's top
 * row, blocks 1-11 from the camera's left column take the value of block 12 and blocks 0-10 are
 * redrawn (Map_RedrawBlock).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8.
 * The copy runs from block 11 down, so every block 1-11 ends up equal to block 12 (as in the
 * ROM); the redraw starts at the left column and moves right. Blocks are 32 px. */
#include "xmen2.h"

short Barbarian_StateScrollTiles(Barbarian *self)
{
	short rows, row, col, n;
	register s16 *p;

	for (rows = 8, row = g_CameraY >> 5; rows; rows--, row++) {
		col = g_CameraX >> 5;
		p = &((s16 *)g_FgBlockMap)[((s16 *)g_FgRowTable)[row] + col];
		for (n = 11; n; n--, col++) {
			p[n] = p[n + 1];
			Map_RedrawBlock(col, row, 0);
		}
	}
	SET_STATE_WAIT(self, Barbarian_StateScrollTiles, 60);
	return self->flags & 8;
}
