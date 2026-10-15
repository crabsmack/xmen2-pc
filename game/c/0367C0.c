/* MagnetoBoss_SmashSegment - MAGNETOBOSS smashes the floor in a map column: the type-$14 floor
 * segment there (at the height of its body box's middle row) is freed, its rows become broken
 * blocks ($85) with an explosion each, and rubble flies from them, away from the target.
 *
 * self: the boss.  col: map column (blocks).
 * Rows: from the segment's y1 + 8 to its y2 - 8 (in 32-pixel blocks). Nothing when there is
 * no segment. Rubble: 8 pieces per row, mirrored when the target is left of the boss, falling
 * to y $180, block $85 (MagnetoBoss_SpawnRubble). */
#include "xmen2.h"

void MagnetoBoss_SmashSegment(MagnetoBoss *self, register short col)
{
	short rowEnd;
	long fx;
	unsigned char away;
	register Seg *seg;
	register short top, bottom;
	short row;
	register long fy;
	register long r;

	away = 0;
	top = self->boxTop >> 5;
	bottom = self->boxBottom >> 5;
	row = (top + bottom) >> 1;
	if (seg = Seg_FindInBlock(0x14, col, row)) {
		row = (seg->y1 + 8) >> 5;
		rowEnd = (seg->y2 - 8) >> 5;
		fx = ((col << 5) + 16) << 8;
		fy = ((row << 5) + 16) << 8;
		for (r = row; r <= rowEnd; r++, fy += 0x2000) {
			Map_SetBlockFG(col, r, 0x85, 0);
			Explosion_SpawnSetTimerWord(fx, fy, 0, 0);
		}
		Seg_Free(seg);
		if (self->target->x < self->x)
			away = 1;
		MagnetoBoss_SpawnRubble(col, row, rowEnd, 8, away, 0x180, 0x85);
	}
}
