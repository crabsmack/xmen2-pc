/* MagnetoBoss_SmashSegmentStaggered - MagnetoBoss_SmashSegment with the explosions of the rows
 * starting 4 frames apart: the type-$14 floor segment in a map column (at the height of its
 * body box's middle row) is freed, its rows become broken blocks ($85), and rubble flies from
 * them, away from the target.
 *
 * self: the boss.  col: map column (blocks).
 * Rows: from the segment's y1 + 8 to its y2 - 8 (in 32-pixel blocks). Nothing when there is
 * no segment. The explosion delay (Explosion_SpawnSetTimerWord's word) is 0, 4, 8... Rubble:
 * 8 pieces per row, mirrored when the target is left of the boss, falling to y $180, block
 * $85 (MagnetoBoss_SpawnRubble). */
#include "xmen2.h"

void MagnetoBoss_SmashSegmentStaggered(MagnetoBoss *self, register short col)
{
	short row, rowEnd;
	long fx;
	unsigned char away;
	register Seg *seg;
	short top, bottom;
	short mid;
	register long fy;
	register long delay;
	register long r;

	away = 0;
	top = self->boxTop >> 5;
	bottom = self->boxBottom >> 5;
	mid = (top + bottom) >> 1;
	if (seg = Seg_FindInBlock(0x14, col, mid)) {
		row = (seg->y1 + 8) >> 5;
		rowEnd = (seg->y2 - 8) >> 5;
		fx = ((col << 5) + 16) << 8;
		fy = ((row << 5) + 16) << 8;
		delay = 0;
		for (r = row; r <= rowEnd; r++, fy += 0x2000) {
			Map_SetBlockFG(col, r, 0x85, 0);
			Explosion_SpawnSetTimerWord(fx, fy, 0, delay);
			delay += 4;
		}
		Seg_Free(seg);
		if (self->target->x < self->x)
			away = 1;
		MagnetoBoss_SpawnRubble(col, row, rowEnd, 8, away, 0x180, 0x85);
	}
}
