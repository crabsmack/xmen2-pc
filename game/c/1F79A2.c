/* Seg_ComputeLength - order a floor segment's endpoints left to right and compute its length.
 *
 * seg: the segment. Its type hook (g_SegTypeHooks, message 4) runs first and does it all when
 * it returns non-zero. Else the endpoints are swapped when x1 > x2 (not with g_SegFlag38C4
 * set), and length = |dx| for flat floors (types $13-$1C, Seg_ComputeLength_Flat), else the
 * larger of |dx| and |dy| (Seg_ComputeLength_Max). */
#include "xmen2.h"

void Seg_ComputeLength(Seg *seg)
{
	register Seg *s = seg;
	short dx, dy;

	if ((g_SegHookTmp = g_SegTypeHooks[s->type]) ? g_SegHookTmp(4, s, 0, 0) : 0)
		return;
	if (s->x1 > s->x2 && !g_SegFlag38C4) {
		dx = s->x1;
		s->x1 = s->x2;
		s->x2 = dx;
		dx = s->y1;
		s->y1 = s->y2;
		s->y2 = dx;
	}
	dx = s->x1 - s->x2;
	dx = ABS(dx);
	dy = s->y1 - s->y2;
	dy = ABS(dy);
	switch (s->type) {
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_ComputeLength_Flat */
		s->length = dx;
		break;
	default:
		/* Seg_ComputeLength_Max */
		s->length = MAX(dx, dy);
		break;
	}
}
