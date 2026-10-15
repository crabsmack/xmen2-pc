/* FlyBrood_MoveBallistic - FLYBROOD mover (+$9E) of the dives: speed moves it, acceleration
 * changes the speed; a dive that started moving vertically stops at y anchorY.
 *
 * self:  the object.
 * steps: movement steps this frame (at least 1).
 * The stop test uses the y speed from before the steps: moving down, it stops once y >= anchorY;
 * up, once y <= anchorY. */
#include "xmen2.h"

void FlyBrood_MoveBallistic(FlyBrood *self, short steps)
{
	short vy = self->vy;

	do {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	} while (--steps);
	if (vy) {
		short ty = self->anchorY;
		short y = self->y >> 8;

		if (vy > 0 && y >= ty || vy < 0 && y <= ty) {
			self->vy = 0;
			self->y = ty << 8;
		}
	}
}
