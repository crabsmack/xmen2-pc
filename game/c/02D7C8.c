/* Exodus_Move - EXODUS Move (class slot +$4E; replaces the parent's): motion by moveFlags.
 *
 * self: the boss.
 * Bit 1: baseX += vx, vx += ax clamped to +-maxVx, x = baseX. Bit 0: the same in y.
 * Bit 2: swayAngleX += swayStepX (wrapped to 0-359), x += sin(swayAngleX) >> swayShiftX.
 * Bit 3: the same for y. No bit set: the base follows the position.
 * Bug kept: a negative swayAngleX adds 360 to swayAngleY instead (the steps are positive). */
#include "xmen2.h"

void Exodus_Move(register Exodus *self)
{
	register long flags = self->moveFlags;

	if (flags) {
		if (flags & 2) {
			self->baseX += self->vx;
			self->vx += self->ax;
			if (self->vx > self->maxVx)
				self->vx = self->maxVx;
			else if (self->vx < -self->maxVx)
				self->vx = -self->maxVx;
			self->x = self->baseX;
		}
		if (flags & 1) {
			self->baseY += self->vy;
			self->vy += self->ay;
			if (self->vy > self->maxVy)
				self->vy = self->maxVy;
			else if (self->vy < -self->maxVy)
				self->vy = -self->maxVy;
			self->y = self->baseY;
		}
		if (flags & 4) {
			self->swayAngleX += self->swayStepX;
			if (self->swayAngleX >= 360)
				self->swayAngleX -= 360;
			else if (self->swayAngleX < 0)
				self->swayAngleY += 360;
			self->x += Math_SinDeg(self->swayAngleX) >> self->swayShiftX;
		}
		if (flags & 8) {
			self->swayAngleY += self->swayStepY;
			if (self->swayAngleY >= 360)
				self->swayAngleY -= 360;
			else if (self->swayAngleY < 0)
				self->swayAngleY += 360;
			self->y += Math_SinDeg(self->swayAngleY) >> self->swayShiftY;
		}
	} else {
		self->baseX = self->x;
		self->baseY = self->y;
	}
}
