/* FlyBrood_ActHover90 - FLYBROOD AI-script action: stop and hover where it is (FlyBrood_MovePath
 * on Tbl_FlyBroodHoverPath) for 90 frames, facing the player (FlyBrood_StateHoverFacePlayer).
 *
 * self: the object. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short FlyBrood_ActHover90(FlyBrood *self)
{
	self->mover = FlyBrood_MovePath;
	self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodHoverPath);
	self->anchorX = self->x >> 8;
	self->anchorY = self->y >> 8;
	STOP_MOVING(self);
	self->countdown = 90;
	SET_STATE(self, (StateFn)FlyBrood_StateHoverFacePlayer);
	return self->flags & 8;
}
