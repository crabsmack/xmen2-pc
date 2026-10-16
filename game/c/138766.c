/* LineSegKiller_RemoveSegsInBox - remove every in-use floor segment of a type (or of any type) that
 * lies inside a box: freed (flags 0, no next owned segment) and unlinked from the segments before
 * and after it.
 *
 * type: segment type, $3E9: any.  left, top, right, bottom: the box (pixels), 5 px larger on
 * every side.
 * Scans g_Segs up to g_SegHighWater; a segment is inside when both endpoints are (x1/y1 taken as
 * its low corner). */
#include "xmen2.h"

void LineSegKiller_RemoveSegsInBox(register short type, register short left, register short top,
	short right, short bottom)
{
	register Seg *seg;
	register short i;

	left -= 5;
	top -= 5;
	right += 5;
	bottom += 5;
	for (seg = g_Segs, i = 0; i <= g_SegHighWater; i++, seg++) {
		if (seg->flags & 0x80 && (type == seg->type || type == 0x3E9)
		    && left <= seg->x1 && right >= seg->x2 && top <= seg->y1 && bottom >= seg->y2) {
			seg->flags = 0;
			seg->nextOwned = 0xFF;
			if (seg->prev != 0xFF) {
				g_Segs[seg->prev].next = 0xFF;
				seg->prev = 0xFF;
			}
			if (seg->next != 0xFF) {
				g_Segs[seg->next].prev = 0xFF;
				seg->next = 0xFF;
			}
		}
	}
}
