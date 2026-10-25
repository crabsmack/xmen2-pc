/* FlyBrood_StateWaitPlayerInRange - FLYBROOD state after FlyBrood_ActHoverUntilNear: face the
 * nearest player; once a live player's body box reaches into its range (232 px to its right
 * when watchRight, else to its left; 56 px above and below it), the next step in 30 frames.
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_StateWaitPlayerInRange(FlyBrood *self)
{
	short top = (self->y >> 8) - 0x38;
	short bottom = top + 0x70;
	XPlayer *p;
	short left, right;
	long i;

	p = Player_Nearest((Actor *)self);
	if (self->watchRight) {
		left = self->x >> 8;
		right = left + 0xE8;
	} else {
		right = self->x >> 8;
		left = right - 0xE8;
	}
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p)) && left <= p->boxRight
		    && right >= p->boxLeft && top <= p->boxBottom && bottom >= p->boxTop) {
			AI_NEXT_STEP(self, self->ai, 30);
			return;
		}
	}
}
