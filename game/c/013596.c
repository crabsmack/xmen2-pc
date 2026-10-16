/* CapedAcolyte_Move - CAPEDACOLYTE Move (class slot +$4E): the velocity moves the home point,
 * kept inside the acolyte's box; the position is home plus a hover wobble.
 *
 * self:  the acolyte.
 * steps: frames to move (at least one step is made).
 * Per frame the velocity is added to home; between frames the hover table advances (restarted
 * at its end marker). No acceleration. Home is then clamped to xMin..xMax / yMin..yMax (pixels);
 * an axis that hits its limit stops (speed and acceleration 0). Position = home + the current
 * hover step, and the table advances once more. */
#include "xmen2.h"

void CapedAcolyte_Move(register CapedAcolyte *self, register short steps)
{
	register short x;
	register short y;

step:
	self->homeX += self->vx;
	self->homeY += self->vy;
	steps--;
	if (steps > 0) {
		self->hover++;
		if (self->hover->dx >= 0x6400)
			self->hover = (HoverStep *)CODEPTR(Tbl_CapedAcolyteHoverOffsets);
		goto step;
	}
	x = self->homeX >> 8;
	if (x < self->xMin) {
		self->homeX = (long)self->xMin << 8;
		self->ax = 0;
		self->vx = 0;
	} else if (x > self->xMax) {
		self->homeX = (long)self->xMax << 8;
		self->ax = 0;
		self->vx = 0;
	}
	y = self->homeY >> 8;
	if (y < self->yMin) {
		self->homeY = (long)self->yMin << 8;
		self->vy = 0;
		self->ay = 0;
	} else if (y > self->yMax) {
		self->homeY = (long)self->yMax << 8;
		self->vy = 0;
		self->ay = 0;
	}
	self->x = self->homeX;
	self->y = self->homeY;
	self->x += self->hover->dx;
	self->y += self->hover->dy;
	self->hover++;
	if (self->hover->dx >= 0x6400)
		self->hover = (HoverStep *)CODEPTR(Tbl_CapedAcolyteHoverOffsets);
}
