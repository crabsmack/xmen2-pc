/* Seg_FindContaining - find the first floor segment of a type whose box contains a point,
 * scanning grid cells downward.
 *
 * x, y: the point (pixels); type: the segment type wanted, 1001 any, 1000 the types $21-$24.
 * The scan of Seg_FindNearestOfType, but the first in-use segment of that type whose bounding
 * box (widened to at least 6 px each way) contains the point wins. Returns it, or 0. */
#include "xmen2.h"

Seg *Seg_FindContaining(short x, short y, short type)
{
	register u8 *p;
	register Seg *s;
	short right;
	short left, top, bottom;
	register long t;
	short cy = y;
	short cell = Seg_GridCell(x, y);
	short prevCell;
	unsigned char i;
	Seg *found;

	found = s = 0;
	do {
		p = &g_SegGridPool[cell];
		prevCell = cell;
		i = *p;
		while (i != 0xFF) {
			s = &g_Segs[i];
			t = s->type;
			if ((s->flags & 0x80) && (type == t || type == 1001
			    || type == 1000 && (t == 0x21 || t == 0x22 || t == 0x23 || t == 0x24))) {
				left = s->x1;
				right = s->x2;
				top = s->y1;
				bottom = s->y2;
				if (left > right) {
					left ^= right;
					right ^= left;
					left ^= right;
				}
				if (bottom < top) {
					top ^= bottom;
					bottom ^= top;
					top ^= bottom;
				}
				if (right - left < 6) {
					left -= 3;
					right = left + 6;
				}
				if (bottom - top < 6) {
					top -= 3;
					bottom = top + 6;
				}
				if (left <= x && x <= right && top <= y && bottom >= y) {
					found = s;
					break;
				}
			}
			i = *++p;
		}
		cy += 1 << g_SegGridShiftY;
	} while ((cell = Seg_GridCell(x, cy)) != prevCell && !found);
	return found;
}
