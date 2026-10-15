/* FlyBrood_StateWaitPlayerPassed - FLYBROOD state (script Tbl_FlyBrood_ScrStepsWaitPassedLen):
 * face the nearest player, stop keeping x and y in range; once the player is on its watched side
 * (right when watchRight, else left), dive (FlyBrood_ActDiveAtPlayer).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_StateWaitPlayerPassed(FlyBrood *self)
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
			SET_STATE(self, FlyBrood_ActDiveAtPlayer);
			return self->flags & 8;
		}
	} else {
		if (self->watchRight) {
			SET_STATE(self, FlyBrood_ActDiveAtPlayer);
			return self->flags & 8;
		}
	}
	SET_STATE(self, FlyBrood_StateWaitPlayerPassed);
	return self->flags & 8;
}
