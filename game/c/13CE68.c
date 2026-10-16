/* Ninja_FindSegInBox - the first floor segment in use, listed in the grid cell of a point, whose
 * bounding box meets a box.
 *
 * x, y: the point (pixels) choosing the grid cell (Seg_GridCell).  layer: the highest segment
 * layer that counts.  unused: not read (Ninja_ClipToSegs passes vy >> 8, + 1).
 * left, top, right, bottom: the box (pixels).
 * The cell's list in g_SegGridPool is g_Segs indices ended by -1. Returns the segment, or 0. */
#include "xmen2.h"

Seg *Ninja_FindSegInBox(short x, short y, short layer, short unused, short left, short top,
	short right, short bottom)
{
	register short cell;
	register u8 *p;
	register char i;
	register Seg *s;
	short minX;
	register short maxX, minY, maxY;

	cell = (short)Seg_GridCell(x, y);
	p = &g_SegGridPool[cell];
	i = *p;
	s = 0;
	while (i != -1) {
		s = &g_Segs[i];
		minX = MIN(s->x1, s->x2);
		maxX = MAX(s->x1, s->x2);
		minY = MIN(s->y1, s->y2);
		maxY = MAX(s->y1, s->y2);
		if (s->layer <= layer && (s->flags & 0x80) && maxX >= left && right >= minX
		    && maxY >= top && minY <= bottom)
			return s;
		i = *++p;
	}
	return 0;
}
