/* Seg_ComputeSlope - compute a floor segment's slope (8 fraction bits).
 *
 * seg: the segment. Flat types $13-$1C (Seg_ComputeSlope_Flat): 0. Others
 * (Seg_ComputeSlope_Calc): dy * 256 / dx while |dy| <= |dx| (0 for a point); steeper than 45
 * degrees, dx * 256 / dy and flags bit 3. */
#include "xmen2.h"

void Seg_ComputeSlope(Seg *seg)
{
	register Seg *s = seg;
	long dx, dy;

	switch (s->type) {
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_ComputeSlope_Flat */
		s->slope = 0;
		break;
	default:
		/* Seg_ComputeSlope_Calc */
		dx = s->x2 - s->x1;
		dy = s->y2 - s->y1;
		if (ABS(dx) >= ABS(dy)) {
			if (dx)
				s->slope = (short)((dy << 8) / dx);
			else
				s->slope = 0;
		} else {
			s->slope = (short)((dx << 8) / dy);
			s->flags |= 8;
		}
		break;
	}
}
