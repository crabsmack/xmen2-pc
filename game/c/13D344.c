/* Ninja_ActLeap - NINJA script action: leap forward (crouch first: Tbl_NinjaAnimCrouch, then
 * Ninja_StateLeapAir when the animation ends) unless that would take it within 50 px of the end
 * of its patrol range it faces; then the next script step from the next update instead.
 *
 * self: the object.
 * Counts a leap off leaveCount while that is above 0 (also when it doesn't leap). The leap: vx
 * $4D4 the way it faces, vy -$540, gravity $56.
 * Returns flags bit 3 when it leaps; no value otherwise. */
#include "xmen2.h"

short Ninja_ActLeap(register Ninja *self)
{
	register short x = self->x >> 8;

	if (self->leaveCount > 0)
		self->leaveCount--;
	if (x <= self->patrolLeft + 0x32 && (self->spr.flags & 0x10)
	    || x >= self->patrolRight - 0x32 && !(self->spr.flags & 0x10)) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		Obj_SetAnim(self, CODEPTR(Tbl_NinjaAnimCrouch));
		self->vx = self->spr.flags & 0x10 ? -0x4D4 : 0x4D4;
		self->vy = -0x540;
		self->ay = 0x56;
		SET_STATE_WAIT(self, Ninja_StateLeapAir, 0xFF);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
