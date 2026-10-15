/* Boulder_FindFloorBelow - pick the floor a BOULDER will land on: of the floor segments listed in
 * its grid cell (Seg_GridCell, g_SegGridPool), the nearest in-use type-3 segment whose first
 * end is at or below it and whose x range holds its x; its layer becomes the boulder's (the
 * floor-segment layer it lands on, Seg_FindAt). None: layer 0.
 *
 * self: the boulder.
 * The height is the segment's y1 (its first end), not the height at x. */
#include "xmen2.h"

void Boulder_FindFloorBelow(register Boulder *self)
{
	short y;
	short n;
	short left, right;
	register Seg *seg;
	register short x;
	register short cell;
	register short best;
	register short dy;

	x = self->x >> 8;
	y = self->y >> 8;
	cell = Seg_GridCell(x, y);
	best = 10000;
	self->layer = 0;
	while ((n = g_SegGridPool[cell]) != 0xFF) {
		seg = &g_Segs[n];
		left = MIN(seg->x1, seg->x2);
		right = MAX(seg->x1, seg->x2);
		dy = seg->y1 - y;
		if (dy >= 0 && seg->flags & 0x80 && x >= left && x <= right && seg->type == 3
				&& dy < best) {
			self->layer = seg->layer;
			best = dy;
		}
		cell++;
	}
}
