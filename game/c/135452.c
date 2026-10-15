/* FlyBrood_StateWaitPlayerPassedLow - FLYBROOD state (script
 * Tbl_FlyBrood_ScrStepsWaitPassedLowLen): as FlyBrood_StateWaitPlayerPassed, then the low dive
 * (FlyBrood_ActDiveAtPlayerLow).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_StateWaitPlayerPassedLow(FlyBrood *self)
{
	XPlayer *p;
	char left;

	p = Player_Nearest((Actor *)self);
	left = self->x > p->x;
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->clampFlags &= ~3;
	if (left) {
		if (!self->watchRight) {
			SET_STATE(self, FlyBrood_ActDiveAtPlayerLow);
			return self->flags & 8;
		}
	} else {
		if (self->watchRight) {
			SET_STATE(self, FlyBrood_ActDiveAtPlayerLow);
			return self->flags & 8;
		}
	}
	SET_STATE(self, FlyBrood_StateWaitPlayerPassedLow);
	return self->flags & 8;
}
