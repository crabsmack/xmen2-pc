/* LaserCannon_StateWatchTarget - LASERCANNON state while it watches its target: when the centre of
 * its own body box is within 200 px in x and 60 px in y of the target's body box, and the target
 * is on the side it faces, LaserCannon_StateExtend from the next update. Else a tracking cannon
 * turns to the target, and once the countdown (60 from LaserCannon_StatePickTarget) is out,
 * LaserCannon_StatePickTarget from the next update; until then it stays the state.
 *
 * self: the cannon. Returns flags & 8 on a change of state, no value otherwise. */
#include "xmen2.h"

short LaserCannon_StateWatchTarget(register LaserCannon *self)
{
	register Actor *t = self->target;
	short left = t->boxLeft - 200;
	short top = t->boxTop - 60;
	register short right = t->boxRight + 200;
	register short bottom = t->boxBottom + 60;
	register long cx = ((self->boxRight + self->boxLeft) << 7) >> 8;
	register long cy = ((self->boxTop + self->boxBottom) << 7) >> 8;

	if (cx >= left && cx <= right && cy >= top && cy <= bottom) {
		if (self->spr.flags & 0x10) {
			if (self->x > t->x) {
				SET_STATE(self, (StateFn)LaserCannon_StateExtend);
				return self->flags & 8;
			}
		} else if (self->x < t->x) {
			SET_STATE(self, (StateFn)LaserCannon_StateExtend);
			return self->flags & 8;
		}
	}
	if (self->tracks) {
		if (self->target->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
	}
	if (!self->countdown) {
		SET_STATE(self, (StateFn)LaserCannon_StatePickTarget);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
