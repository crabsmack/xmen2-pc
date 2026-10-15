/* FlyBrood_MoveHoming - FLYBROOD mover (+$9E): home in on a point by the nearest player while a
 * path adds velocity changes (the swoop and homing paths).
 *
 * self:  the object.
 * steps: movement steps this frame.
 * The target is (player x + anchorX, player's second box top + anchorY), taken once. The
 * acceleration of the last call becomes the speed and is cleared; each step then adds the
 * path step's change (dx mirrored unless heading right) and goes to the next step: one with dx
 * or dy above $100 ends the path (dx $3E9: back dy steps instead), and half the distance to the
 * target, halved until |dx| + |dy| <= 4, is added to the acceleration. Returns 0.
 * Never runs in the corpus. */
#include "xmen2.h"

long FlyBrood_MoveHoming(FlyBrood *self, short steps)
{
	XPlayer *p;
	register short tx, ty;
	register short dx, dy;

	p = Player_Nearest((Actor *)self);
	tx = (p->x >> 8) + self->anchorX;
	ty = p->box2Top + self->anchorY;
	self->vx = self->ax;
	self->vy = self->ay;
	self->ax = 0;
	self->ay = 0;
	do {
		if (self->path) {
			if (self->headingRight)
				self->vx += self->path->dx << 8;
			else
				self->vx -= self->path->dx << 8;
			self->vy += self->path->dy << 8;
			self->path++;
			if ((self->path->dx > 0x100 || self->path->dy > 0x100) ? 1 : 0) {
				if (self->path->dx == 0x3E9)
					self->path -= self->path->dy;
				else
					self->path = 0;
			}
		}
		dx = (tx - (self->x >> 8)) >> 1;
		dy = (ty - (self->y >> 8)) >> 1;
		if (dx || dy) {
			for (;;) {
				if (ABS(dx) + ABS(dy) <= 4)
					break;
				dx >>= 1;
				dy >>= 1;
			}
			self->ax += dx << 8;
			self->ay += dy << 8;
		}
		steps--;
	} while (steps > 0);
	return 0;
}
