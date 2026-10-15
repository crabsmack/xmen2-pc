/* Seg_FindNearestOfType - find the floor segment of a type nearest a point, scanning grid cells
 * downward until a cell has one.
 *
 * x, y: the point (pixels); type: the segment type wanted, 1001 any, 1000 the types $21-$24.
 * Score of an in-use segment of that type: the distance from y to the middle of its height plus
 * the distance from x to its x range (0 inside); its box is first widened to at least 6 px each
 * way (around its first corner less 3). The lowest score under 20000 in a cell wins; the scan goes
 * on one cell height (1 << g_SegGridShiftY) down while none is found and the cell changes.
 * Returns it, or 0. Endpoints are ordered with xor swaps (the original's). */
#include "xmen2.h"

Seg *Seg_FindNearestOfType(short x, short y, short type)
{
	register u8 *p;
	register Seg *s;
	short left, top, bottom;
	register long t;
	short right;
	short score;
	short best = 20000;
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
				score = ABS(((top + bottom) >> 1) - y)
					+ (x < s->x1 ? s->x1 - x : x < s->x2 ? 0 : x - s->x2);
				if (score < best) {
					best = score;
					found = s;
				}
			}
			i = *++p;
		}
		cy += 1 << g_SegGridShiftY;
	} while ((cell = Seg_GridCell(x, cy)) != prevCell && !found);
	return found;
}
