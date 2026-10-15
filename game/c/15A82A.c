/* Seg_FindAt - find a floor segment in the point's grid cell whose top lies just above the point.
 *
 * x, y: the point (pixels); maxLayer: only segments of layers up to it; height: the band.
 * Walks the cell's list (Seg_GridCell) for the first in-use segment that spans x and whose top
 * (the smaller y) is at most y and above y - height. Returns it, or 0. */
#include "xmen2.h"

Seg *Seg_FindAt(short x, short y, short maxLayer, short height)
{
	register u8 *p;
	register Seg *s;
	short off;
	unsigned char i;
	short left, right;
	short top;

	off = Seg_GridCell(x, y);
	p = &g_SegGridPool[off];
	i = *p;
	s = 0;
	while (i != 0xFF) {
		s = &g_Segs[i];
		left = s->x1;
		right = s->x2;
		if (left > right) {
			left = s->x2;
			right = s->x1;
		}
		top = MIN(s->y1, s->y2);
		if (s->layer <= maxLayer && (s->flags & 0x80) && top <= y && y < top + height
		    && left <= x && right >= x)
			return s;
		i = *++p;
	}
	return 0;
}
