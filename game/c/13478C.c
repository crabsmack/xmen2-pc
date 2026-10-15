/* FlyBrood_MovePath - FLYBROOD mover (+$9E): place it on the anchor plus the path's offset
 * (>> shift) for each step; the path (Tbl_FlyBroodHoverPath) loops at its $7F end.
 *
 * self:  the object.
 * steps: movement steps this frame (at least 1). */
#include "xmen2.h"

void FlyBrood_MovePath(FlyBrood *self, short steps)
{
	short dx, dy, shift;

	do {
		dx = self->path->dx;
		dy = self->path->dy;
		shift = self->shift;
		dx = dx >> shift;
		dy >>= shift;
		self->x = (self->anchorX + dx) << 8;
		self->y = (self->anchorY + dy) << 8;
		self->path++;
		if (self->path->dx == 0x7F)
			self->path = (FlyPathStep *)CODEPTR(Tbl_FlyBroodHoverPath);
	} while (--steps);
}
