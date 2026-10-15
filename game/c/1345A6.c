/* FlyBrood_EnterFromSide - put a FLYBROOD just off a side of the screen at mid height, facing
 * in, and set its homing offset (FlyBrood_MoveHoming aims 130 px short of the player, 27 px
 * above the player's second box).
 *
 * self:     the object.
 * fromLeft: nonzero: 32 px left of the screen, heading right; 0: 32 px right of it ($160),
 *           heading left. Never runs in the corpus. */
#include "xmen2.h"

void FlyBrood_EnterFromSide(FlyBrood *self, char fromLeft)
{
	self->headingRight = fromLeft;
	if (fromLeft) {
		self->x = (g_CameraX - 0x20) << 8;
		self->y = (g_CameraY + 0x82) << 8;
		self->spr.flags &= ~0x10;
	} else {
		self->x = (g_CameraX + 0x160) << 8;
		self->y = (g_CameraY + 0x82) << 8;
		self->headingRight = 0;
		self->spr.flags |= 0x10;
	}
	self->anchorX = self->headingRight ? -0x82 : 0x82;
	self->anchorY = -0x1B;
}
