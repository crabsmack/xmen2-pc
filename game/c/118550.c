/* ICandy_Update - ICANDY Update (class slot +$12, its timer every frame): leave the active window
 * and it is destroyed; else move it (position += velocity, velocity += acceleration; twice in
 * two-player stepping) and step its animation; with no animation (none, or a one-shot that
 * ended) it is destroyed (class slot +$1A).
 *
 * self: the object.
 * An animation step lasts the script's delay in frames (counted 1 a frame, 2 in two-player
 * stepping); a new frame whose tiles are not the uploaded ones (or none, $FF) clears the
 * sprite's uploaded flag (bit 5). The ended one-shot is destroyed on the next frame. */
#include "xmen2.h"

#define SCRIPT(o)	((AnimScript *)(u8 *)(o)->anim)	/* the animation header */

void ICandy_Update(register ICandy *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;
	char out = 0;

	if (sx > g_DeactWinRight)
		out = 1;
	else if (sx < g_DeactWinLeft)
		out = 1;
	else if (sy > g_DeactWinBottom)
		out = 1;
	else if (sy < g_DeactWinTop)
		out = 1;
	if (out) {
		(*self->cls->method[5])(self);		/* +$1A Destroy */
		return;
	}
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (g_TwoPlayerStep) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
	if (!self->anim) {
		(*self->cls->method[5])(self);		/* +$1A Destroy */
		return;
	}
	self->animTime += (char)(g_TwoPlayerStep ? 2 : 1);
	if (self->animTime >= SCRIPT(self)->delay) {
		self->animStep++;
		if (SCRIPT(self)->count <= self->animStep) {
			self->animStep = 0;
			if (SCRIPT(self)->flags & 2)
				self->anim = 0;
		}
		if (self->anim) {
			self->frame = *(self->anim + (self->animStep << 1) + 7);
			if (self->frame == 0xFF || self->frame != self->uploaded)
				self->sprFlags &= ~0x20;
			self->animTime = 0;
		}
	}
}
