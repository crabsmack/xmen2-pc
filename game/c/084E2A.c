/* SmallBrood_StateJumpUntilSurface - SMALLBROOD state: fly on until y is the floor or the
 * ceiling, then the landing animation (SmallBrood_StateLanded when it ends).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateJumpUntilSurface(SmallBrood *self)
{
	short y;

	y = self->y >> 8;
	if (y == self->floorY || y == self->ceilY) {
		Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim6));
		SET_STATE_WAIT(self, (StateFn)SmallBrood_StateLanded, 0xFF);
		return self->flags & 8;
	}
	SET_STATE(self, SmallBrood_StateJumpUntilSurface);
	return self->flags & 8;
}
