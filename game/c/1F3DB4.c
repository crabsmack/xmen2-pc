/* XClimber_ClassifySeg - XCLIMBER ClassifySeg (class slot +$F2): the grab a touched segment allows:
 * 0 none, 1 a ceiling, 2 a wall on the player's right, 3 a wall on the left.
 *
 * self: the player.  seg: the segment (0: none).
 * By segment type: 1 and 2 are walls (2 and 3) at least $3E px tall; 3 never; 4 a ceiling at
 * least $3E px long; $13 (a solid block) by the side the player's body box is on: left of it a
 * wall 2, right of it 3, below it the ceiling (each with the same size limits). */
#include "xmen2.h"

short XClimber_ClassifySeg(register XClimber *self, register Seg *seg)
{
#ifdef XMEN2_MPW
	register short w = seg->x2 - seg->x1;
	register short h = seg->y2 - seg->y1;
#else
	/* with no segment the 68000 reads the vector table here; the sizes go unused then */
	register short w = seg ? seg->x2 - seg->x1 : 0;
	register short h = seg ? seg->y2 - seg->y1 : 0;
#endif

	w = ABS(w);
	h = ABS(h);
	if (!seg)
		return 0;
	switch (seg->type) {
	case 3:
		return 0;
	case 4:
		if (1) {
			if (w < 0x3E)
				return 0;
			return 1;
		}
	case 1:
		if (h < 0x3E)
			return 0;
		return 2;
	case 2:
		if (h < 0x3E)
			return 0;
		return 3;
	case 0x13:
		if (self->boxLeft >= seg->x2) {
			if (h < 0x3E)
				return 0;
			return 2;
		}
		if (self->boxRight <= seg->x1) {
			if (h < 0x3E)
				return 0;
			return 3;
		}
		if (self->boxTop >= seg->y2) {
			if (w < 0x3E)
				return 0;
			return 1;
		}
		return 0;
	default:
		return 0;
	}
}
