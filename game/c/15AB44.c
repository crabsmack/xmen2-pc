/* Seg_FindLeft - find the nearest floor segment of a type to the left of a point.
 *
 * x, y: the point (pixels); type: the segment type wanted.
 * Seg_FindAbove along x: in each grid cell from the point's leftward (one cell width,
 * 1 << g_SegGridShiftX, at a time), the in-use segment of that type spanning y whose right end
 * (the larger x) is left of x and nearest (under 5000 px) wins; the scan stops at the first
 * cell with one or when the cell no longer changes. Returns it, or 0. */
#include "xmen2.h"

Seg *Seg_FindLeft(short x, short y, short type)
{
	register u8 *p;
	register Seg *s;
	unsigned char i;
	short top, bottom;
	short dist;
	short cell = Seg_GridCell(x, y);
	short prevCell;
	short right;
	short cx;
	short best = 5000;
	Seg *found;

	found = s = 0;
	cx = x;
	do {
		p = &g_SegGridPool[cell];
		prevCell = cell;
		i = *p;
		while (i != 0xFF) {
			s = &g_Segs[i];
			top = s->y1;
			bottom = s->y2;
			if (top > bottom) {
				top = s->y2;
				bottom = s->y1;
			}
			right = MAX(s->x1, s->x2);
			dist = x - right;
			if (dist > 0 && dist < best && s->type == type && (s->flags & 0x80) && top <= y
			    && bottom >= y) {
				found = s;
				best = dist;
			}
			i = *++p;
		}
		cx -= 1 << g_SegGridShiftX;
	} while ((cell = Seg_GridCell(cx, y)) != prevCell && !found);
	return found;
}
