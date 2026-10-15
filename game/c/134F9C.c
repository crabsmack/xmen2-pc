/* FlyBrood_ActHoverUntilNear - FLYBROOD AI-script action: stop and hover where it is
 * (FlyBrood_MovePath on Tbl_FlyBroodHoverPath) until a player comes into range
 * (FlyBrood_StateWaitPlayerInRange).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_ActHoverUntilNear(FlyBrood *self)
{
	self->mover = FlyBrood_MovePath;
	self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodHoverPath);
	self->anchorX = self->x >> 8;
	self->anchorY = self->y >> 8;
	STOP_MOVING(self);
	SET_STATE(self, (StateFn)FlyBrood_StateWaitPlayerInRange);
	return self->flags & 8;
}
