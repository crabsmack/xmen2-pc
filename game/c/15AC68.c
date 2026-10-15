/* Seg_FindRight - find the nearest floor segment of a type to the right of a point.
 *
 * x, y: the point (pixels); type: the segment type wanted.
 * Seg_FindLeft mirrored: in each grid cell from the point's rightward, the in-use segment of
 * that type spanning y whose left end (the smaller x) is right of x and nearest (under 5000 px)
 * wins; the scan stops at the first cell with one or when the cell no longer changes. Returns
 * it, or 0. left is stored, never read (the original's). */
#include "xmen2.h"

Seg *Seg_FindRight(short x, short y, short type)
{
	register u8 *p;
	register Seg *s;
	unsigned char i;
	short top, bottom;
	short dist;
	short cell = Seg_GridCell(x, y);
	short prevCell;
	short left;
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
			left = dist = MIN(s->x1, s->x2);
			dist -= x;
			if (dist > 0 && dist < best && s->type == type && (s->flags & 0x80) && top <= y
			    && bottom >= y) {
				found = s;
				best = dist;
			}
			i = *++p;
		}
		cx += 1 << g_SegGridShiftX;
	} while ((cell = Seg_GridCell(cx, y)) != prevCell && !found);
	return found;
}
