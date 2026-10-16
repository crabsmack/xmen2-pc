/* PowerUp_Update - POWERUP's Update (class slot +$4A, in place of ACTOR's): its lifetime counts down
 * (by 2 in two-player stepping) and at its end it is killed (+$52 Kill); outside the active window
 * (g_DeactWin*, relative to the camera) it is deactivated (+$46), and goes on only if still present.
 * A live player whose body box holds its position, with health above 0 and below $900, collects it
 * (+$6E Collect, then Kill, sound $39). Else its animation steps (the script's delay per step, a
 * one-shot script ends; the frame shown, its tiles uploaded again when it changed).
 *
 * self: the power-up. */
#include "xmen2.h"

void PowerUp_Update(register PowerUp *self)
{
	register short px = self->x >> 8;
	register short py = self->y >> 8;
	short sx = px - g_CameraX;
	register short sy = py - g_CameraY;
	register short steps = g_TwoPlayerStep ? 2 : 1;
	register XPlayer *p;
	register short i;

	if (self->lifetime) {
		self->lifetime -= steps;
		if (self->lifetime <= 0) {
			(*self->cls->method[19])(self);	/* +$52 Kill */
			return;
		}
	}
	if (sx > g_DeactWinRight || sx < g_DeactWinLeft || sy > g_DeactWinBottom || sy < g_DeactWinTop) {
		(*self->cls->method[16])(self);	/* +$46 Deactivate */
		if (!(self->flags & 0x8000))
			return;
	}
	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2))
		    && px >= p->boxLeft && px <= p->boxRight && py >= p->boxTop && py <= p->boxBottom
		    && p->health > 0 && p->health < 0x900) {
			(*self->cls->method[26])(self, p);	/* +$6E Collect */
			(*self->cls->method[19])(self);	/* +$52 Kill */
			Snd_Play(0x39);
			return;
		}
	}
	self->animDist += steps;
	if (self->animDist >= ((AnimScript *)self->anim)->delay) {
		++self->animStep;
		if (((AnimScript *)self->anim)->count <= self->animStep) {
			self->animStep = 0;
			if (((AnimScript *)self->anim)->flags & 2)
				self->anim = 0;
		}
		if (self->anim) {
			self->spr.frame = *((u8 *)self->anim + (self->animStep << 1) + 7);
			if (self->spr.frame == 0xFF || self->spr.frame != self->spr.uploaded)
				self->spr.flags &= ~0x20;
			self->animDist = 0;
		}
	}
}
