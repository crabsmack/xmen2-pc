/* MagnetoBoss_DirFromTo - the globe direction (0-9) for the offset of the boss's body box
 * centre from the target's.
 *
 * self: the boss.  t: the target.
 * dx, dy = self's centre - t's (8 fraction bits). Mostly horizontal (|dy| < |dx| / 2): 2 for
 * dx > 0, else 7. Mostly vertical (|dx| < |dy| / 2): dy > 0: 4 (dx > 0) / 5; else 0 / 9.
 * Diagonal: dy > 0: 3 / 6; else 1 / 8. */
#include "xmen2.h"

short MagnetoBoss_DirFromTo(register MagnetoBoss *self, register Actor *t)
{
	long dy;
	register long adx, ady;
	register long dx;
	register short dir;

	dx = ((self->boxRight + self->boxLeft) << 7) - ((t->boxRight + t->boxLeft) << 7);
	dy = ((self->boxTop + self->boxBottom) << 7) - ((t->boxTop + t->boxBottom) << 7);
	adx = dx;
	ady = dy;
	if (adx < 0)
		adx = -adx;
	if (ady < 0)
		ady = -ady;
	if (adx >> 1 > ady) {
		if (dx > 0)
			dir = 2;
		else
			dir = 7;
	} else if (ady >> 1 > adx) {
		if (dy > 0) {
			if (dx > 0)
				dir = 4;
			else
				dir = 5;
		} else if (dx > 0)
			dir = 0;
		else
			dir = 9;
	} else if (dy > 0) {
		if (dx > 0)
			dir = 3;
		else
			dir = 6;
	} else if (dx > 0)
		dir = 1;
	else
		dir = 8;
	return dir;
}
