/* ApocBomb_Move - APOCBOMB Move (class slot +$4E; replaces the parent's).
 *
 * self: the bomb.  steps: frames to move.
 * It orbits the arena on an ellipse: x = 256 - 224 sin a, ground y = 194 + 32 cos a (kept in
 * groundY for the shadow), the angle advancing by g_ApocBombSpeed (twice that in two-player
 * step mode; 8 fraction bits, wrapping at 360). Passing angle 180 (the far side of the orbit,
 * where ApocBomb_Init drops it: ground y 162, against 226 nearest the viewer at angle 0) for
 * the third time expires it (bombFlags bit 2; ApocBomb_Update blows it up) and ends the move.
 * Dropping (bit 0): it falls with gravity $50 until it reaches the ground, then sound $E9 and,
 * unless kind 0, it bounces (bit 1). Otherwise its height above the ground changes by heightStep per step,
 * reversing at 0 and $2800 (kind 1 stops bouncing at the top), and y = ground - height.
 * (A drop moves once per call, whatever steps is.) */
#include "xmen2.h"

void ApocBomb_Move(register ApocBomb *self, register short steps)
{
	register long s = Math_SinDeg(self->angle >> 8);
	register long c = Math_CosDeg(self->angle >> 8);
	register long dx = (s << 8) - (s << 5);		/* 224 sin */
	register long dy = c << 5;
	register long old = self->angle;
	register long ground;

	dx >>= 16;
	dy >>= 16;
	self->x = (256 - dx) << 8;
	self->groundY = dy + 0xC2;
	ground = (long)self->groundY << 8;
	self->speed = g_ApocBombSpeed;
	self->angle += g_TwoPlayerStep ? self->speed << 1 : self->speed;
	if (self->angle >= 0x16800)
		self->angle -= 0x16800;
	else if (self->angle < 0)
		self->angle += 0x16800;
	if (old < 0xB400 && self->angle >= 0xB400) {
		self->passes++;
		if (self->passes >= 3) {
			self->bombFlags |= 4;
			return;
		}
	}
	if (self->bombFlags & 1) {
		if (self->y < ground) {
			self->y += self->vy;
			self->vy += 0x50;
		} else {
			self->bombFlags &= ~1;
			Snd_Stop(0xE9);
			Snd_Play(0xE9);
			if (self->kind)
				self->bombFlags |= 2;
		}
	} else {
		if (self->bombFlags & 2) {
			do
				self->height += self->heightStep;
			while (--steps);
			if (self->height > 0x2800) {
				self->height = 0x2800;
				if (self->kind == 1)
					self->bombFlags &= ~2;
				self->heightStep = -self->heightStep;
			} else if (self->height < 0) {
				self->height = 0;
				self->heightStep = -self->heightStep;
			}
		}
		self->y = ground - self->height;
	}
}
