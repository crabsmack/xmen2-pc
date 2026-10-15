/* BigAcolyte_Move - BIGACOLYTE Move (class slot +$4E, replaces the parent's).
 *
 * self:  the boss.
 * steps: frames to move (at least one).
 * Per frame, while bobbing: y += sin(bobAngle) >> 10, bobAngle += 2 (0-358). Otherwise each axis
 * not yet arrived glides to its target: the acceleration drives the speed until it hits the
 * speed limit (maxVy/minVy, both axes); without acceleration, once the target is within 6
 * frames of travel, it brakes by 1/8 of the speed per frame. Reaching or passing the target
 * snaps the axis to its middle (target px + 0.5), stops it, marks it arrived and sets the
 * countdown to 9. Both arrived, countdown out and not a picture: start bobbing from angle 0. */
#include "xmen2.h"

void BigAcolyte_Move(register BigAcolyte *self, short steps)
{
	register short pos;

	do {
		if (self->bobbing) {
			self->y += Math_SinDeg(self->bobAngle) >> 10;
			self->bobAngle += 2;
			if (self->bobAngle >= 360)
				self->bobAngle = 0;
			continue;
		}
		if (!self->arrivedX) {
			if (self->ax) {
				self->vx += self->ax;
				if (self->vx > self->maxVy) {
					self->vx = self->maxVy;
					self->ax = 0;
				} else if (self->vx < self->minVy) {
					self->vx = self->minVy;
					self->ax = 0;
				}
			} else {
				pos = (self->x + self->vx * 6) >> 8;
				if (self->vx < 0 && pos <= self->targetX || self->vx > 0 && pos >= self->targetX)
					self->ax = -self->vx >> 3;
			}
			self->x += self->vx;
			pos = self->x >> 8;
			if (self->vx < 0) {
				if (pos <= self->targetX) {
					self->x = (long)self->targetX << 8 | 0x80;
					self->ax = 0;
					self->vx = 0;
					self->arrivedX = 1;
					self->countdown = 9;
				}
			} else if (pos >= self->targetX) {
				self->x = (long)self->targetX << 8 | 0x80;
				self->ax = 0;
				self->vx = 0;
				self->arrivedX = 1;
				self->countdown = 9;
			}
		}
		if (!self->arrivedY) {
			if (self->ay) {
				self->vy += self->ay;
				if (self->vy > self->maxVy) {
					self->vy = self->maxVy;
					self->ay = 0;
				} else if (self->vy < self->minVy) {
					self->vy = self->minVy;
					self->ay = 0;
				}
			} else {
				pos = (self->y + self->vy * 6) >> 8;
				if (self->vy < 0 && pos <= self->targetY || self->vy > 0 && pos >= self->targetY)
					self->ay = -self->vy >> 3;
			}
			self->y += self->vy;
			pos = self->y >> 8;
			if (self->vy < 0) {
				if (pos <= self->targetY) {
					self->y = (long)self->targetY << 8 | 0x80;
					self->vy = 0;
					self->ay = 0;
					self->arrivedY = 1;
					self->countdown = 9;
				}
			} else if (pos >= self->targetY) {
				self->y = (long)self->targetY << 8 | 0x80;
				self->vy = 0;
				self->ay = 0;
				self->arrivedY = 1;
				self->countdown = 9;
			}
		}
		if (self->arrivedY && self->arrivedX && self->countdown <= 0 && !self->picture) {
			self->bobbing = 1;
			self->bobAngle = 0;
		}
	} while (--steps > 0);
}
