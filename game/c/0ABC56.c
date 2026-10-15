/* FloatBubble_Move - FLOATBUBBLE Move (class slot +$4E): drift with a wobble.
 *
 * self:  the bubble.
 * steps: 2 in two-player mode (the drift step is then done twice), else 1.
 * Nothing moves until bubbleFlags bit 2 is set; while bit 1 is set it is held at the surface
 * line g_FloodLineY. Otherwise: speed into position, acceleration into speed (keeping the old
 * speed in prevVx/prevVy), speed clamped to +-max. The wobble is an offset added to the
 * position each Move, changed by wobbleAx/Ay and clamped to +-100; wobbleAx/Ay turn back
 * (-4/+4 in x, -3/+3 in y) once the bubble is more than $60 (8 fraction bits) away from its
 * anchor. The anchor follows the bubble on an axis while it moves slower than $10 there or
 * its speed changed sign this Move. */
#include "xmen2.h"

void FloatBubble_Move(FloatBubble *self, short steps)
{
	if (!(self->bubbleFlags & 4))
		return;
	if (self->bubbleFlags & 2) {
		self->y = g_FloodLineY << 8;
		return;
	}
	self->prevVx = self->vx;
	self->prevVy = self->vy;
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (steps > 1) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
	if (self->vx > self->maxVx)
		self->vx = self->maxVx;
	if (self->vx < -self->maxVx)
		self->vx = -self->maxVx;
	if (self->vy > self->maxVy)
		self->vy = self->maxVy;
	if (self->vy < -self->maxVy)
		self->vy = -self->maxVy;
	{
		short dx = self->x - self->anchorX;	/* from the anchor (8 fraction bits) */
		short dy = self->y - self->anchorY;

		self->wobbleX += self->wobbleAx;
		self->wobbleY += self->wobbleAy;
		if (self->wobbleX > 100)
			self->wobbleX = 100;
		else if (self->wobbleX < -100)
			self->wobbleX = -100;
		if (self->wobbleY > 100)
			self->wobbleY = 100;
		else if (self->wobbleY < -100)
			self->wobbleY = -100;
		if (dx > 0x60)
			self->wobbleAx = -4;
		else if (dx < -0x60)
			self->wobbleAx = 4;
		if (dy > 0x60)
			self->wobbleAy = -3;
		else if (dy < -0x60)
			self->wobbleAy = 3;
		if (ABS(self->vx) < 0x10 || ((self->vx & 0x8000) ^ (self->prevVx & 0x8000)) > 0)
			self->anchorX = self->x;
		if (ABS(self->vy) < 0x10 || ((self->vy & 0x8000) ^ (self->prevVy & 0x8000)) > 0)
			self->anchorY = self->y;
	}
	self->x += self->wobbleX;
	self->y += self->wobbleY;
}
