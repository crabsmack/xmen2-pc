/* ShootAcolyte_StateWalkToPlayer - SHOOTACOLYTE state: walk (speed $220) to 96 px short of the
 * nearest player, the target kept inside rangeLeft..rangeRight.
 *
 * self: the object.
 * It stops (no motion, no animation, frame 0, the next script step in 10 frames) when within
 * 10 px of the target, when at a range end with the target beyond it, or when its countdown
 * (walkTime, ShootAcolyte_ActWalk) has run out. Else it faces the target, plays the walk
 * animation Tbl_ShootAcolyteAnim and runs again next frame.
 * Returns flags bit 3 while walking; no value when it stops. */
#include "xmen2.h"

short ShootAcolyte_StateWalkToPlayer(register ShootAcolyte *self)
{
	register XPlayer *p;
	register short x;		/* its x, then the player's, in pixels */
	register short px;
	register short target;

	x = self->x >> 8;
	p = Player_Nearest((Actor *)self);
	px = p->x >> 8;
	if (x < px) {
		target = px - 0x60;
		target = MAX(target, self->rangeLeft);
	} else {
		target = px + 0x60;
		target = MIN(target, self->rangeRight);
	}
	if (ABS(target - x) < 10
	    || x == self->rangeRight && target > self->rangeRight
	    || x == self->rangeLeft && target < self->rangeLeft
	    || self->countdown <= 0) {
		STOP_MOVING(self);
		self->anim = 0;
		(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
		AI_NEXT_STEP(self, self->ai, 10);
	} else {
		if (x < target)
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		Obj_ChangeAnim(self, CODEPTR(Tbl_ShootAcolyteAnim));
		self->vx = self->spr.flags & 0x10 ? -0x220 : 0x220;
		SET_STATE(self, ShootAcolyte_StateWalkToPlayer);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
