/* FlyBrood_StateWaitPlayerPassedSine - FLYBROOD state (script
 * Tbl_FlyBrood_ScrStepsWaitPassedSineLen): as FlyBrood_StateWaitPlayerPassed, then the sine
 * fly-by (FlyBrood_ActSineFlyBy).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_StateWaitPlayerPassedSine(FlyBrood *self)
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
			SET_STATE(self, FlyBrood_ActSineFlyBy);
			return self->flags & 8;
		}
	} else {
		if (self->watchRight) {
			SET_STATE(self, FlyBrood_ActSineFlyBy);
			return self->flags & 8;
		}
	}
	SET_STATE(self, FlyBrood_StateWaitPlayerPassedSine);
	return self->flags & 8;
}
