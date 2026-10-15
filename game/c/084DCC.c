/* SmallBrood_StateJumpFlip - SMALLBROOD state, mid-jump: turn over (mirror y about the body box's
 * centre, toggle the v-flip) and fly on until a surface (SmallBrood_StateJumpUntilSurface).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateJumpFlip(SmallBrood *self)
{
	long d;

	d = (self->y - ((self->boxTop + self->boxBottom) << 7)) << 1;
	self->y -= d;
	self->spr.flags ^= 8;
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim5));
	SET_STATE_WAIT(self, SmallBrood_StateJumpUntilSurface, 0xFF);
	return self->flags & 8;
}
