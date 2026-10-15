/* Seg_FindAbove - find the nearest floor segment above a point, scanning grid cells upward.
 *
 * x, y: the point (pixels); maxLayer: only segments of layers up to it.
 * Seg_FindBelow upside down: in each grid cell from the point's up, the in-use segment spanning
 * x whose lowest point (the larger y) is above y and nearest (under 5000 px) wins; the scan
 * stops at the first cell with one or when the cell no longer changes (the grid's top row).
 * Returns it, or 0. */
#include "xmen2.h"

Seg *Seg_FindAbove(short x, short y, short maxLayer)
{
	register u8 *p;
	register Seg *s;
	unsigned char i;
	short left, right;
	short dist;
	short cell = Seg_GridCell(x, y);
	short prevCell;
	short bottom;
	short cy;
	short best = 5000;
	Seg *found;

	found = s = 0;
	cy = y;
	do {
		p = &g_SegGridPool[cell];
		prevCell = cell;
		i = *p;
		while (i != 0xFF) {
			s = &g_Segs[i];
			left = s->x1;
			right = s->x2;
			if (left > right) {
				left = s->x2;
				right = s->x1;
			}
			bottom = MAX(s->y1, s->y2);
			dist = y - bottom;
			if (dist > 0 && s->layer <= maxLayer && (s->flags & 0x80) && left <= x && right >= x
			    && dist < best) {
				found = s;
				best = dist;
			}
			i = *++p;
		}
		cy -= 1 << g_SegGridShiftY;
	} while ((cell = Seg_GridCell(x, cy)) != prevCell && !found);
	return found;
}
