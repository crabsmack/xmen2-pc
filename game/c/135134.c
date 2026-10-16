/* FlyBrood_ActHoverForever - FLYBROOD AI-script action (its only step in Tbl_FlyBrood_ScriptA,
 * FlyBrood_ScrStepHoverForever): stop and hover where it is (FlyBrood_MovePath on
 * Tbl_FlyBroodHoverPath), facing the player from now on (FlyBrood_StateFacePlayer).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_ActHoverForever(FlyBrood *self)
{
	self->mover = FlyBrood_MovePath;
	self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodHoverPath);
	self->anchorX = self->x >> 8;
	self->anchorY = self->y >> 8;
	STOP_MOVING(self);
	SET_STATE(self, (StateFn)FlyBrood_StateFacePlayer);
	return self->flags & 8;
}
