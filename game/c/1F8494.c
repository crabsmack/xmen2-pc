/* Seg_SetEndpoints - set a floor segment's endpoints and recompute its length.
 *
 * seg: the segment.  x1, y1, x2, y2: the endpoints (pixels).
 * Hand-written 68000 (stack arguments, no frame). */
#include "xmen2.h"

void Seg_SetEndpoints(void *seg, short x1, short y1, short x2, short y2)
{
	Seg *s = (Seg *)seg;

	s->x1 = x1;
	s->y1 = y1;
	s->x2 = x2;
	s->y2 = y2;
	Seg_ComputeLength(s);
}
