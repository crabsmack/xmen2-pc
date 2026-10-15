/* SmallBrood_ActJump - SMALLBROOD AI-script action: jump from the floor to the ceiling, or drop
 * from the ceiling to the floor.
 *
 * self: the object.
 * On the floor: upright, vy -$400; else upside down, vy $400. Jump animation, then
 * SmallBrood_StateJumpFlip when it ends. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_ActJump(SmallBrood *self)
{
	short y;

	y = self->y >> 8;
	self->ax = 0;
	self->vx = 0;
	if (y == self->floorY) {
		self->spr.flags &= ~8;
		self->vy = -0x400;
	} else {
		self->spr.flags |= 8;
		self->vy = 0x400;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim4));
	SET_STATE_WAIT(self, SmallBrood_StateJumpFlip, 0xFF);
	return self->flags & 8;
}
