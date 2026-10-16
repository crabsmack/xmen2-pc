/* Spark_Update - SPARK Update (class slot +$4A; the parent's isn't called): a cut-down ACTOR
 * Update.
 *
 * self: the object.
 * Outside the deactivation window (relative to the camera): Deactivate, and Kill when out of
 * health; done. Else (steps 2 in the two-player step mode, else 1): Move while it has health,
 * the distance-driven animation advances by the movement (a new frame sets flags 8), the state
 * runs when the timer is out (else the timer runs down unless it is $FF or more; gone after the
 * state: done), and flags 8 recomputes the boxes (and is cleared). No countdown, no hit
 * cooldown, no children. Never runs in the corpus. */
#include "xmen2.h"

void Spark_Update(register Spark *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;
	register char out;
	register short steps;
	register long x0, y0;

	out = 0;
	if (sx > g_DeactWinRight)
		out = 1;
	if (sx < g_DeactWinLeft)
		out = 1;
	if (sy > g_DeactWinBottom)
		out = 1;
	if (sy < g_DeactWinTop)
		out = 1;
	if (out) {
		(*self->cls->method[16])(self);			/* +$46 Deactivate */
		if (self->health <= 0)
			(*self->cls->method[19])(self);		/* +$52 Kill */
		return;
	}
	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	x0 = self->x;
	y0 = self->y;
	if (self->health > 0)
		(*self->cls->method[18])(self, steps);		/* +$4E Move */
	if ((char)Anim_AdvanceByDistance(self, steps, self->x - x0, self->y - y0))
		self->flags |= 8;
	if (self->timer > 0) {
		if (self->timer < 0xFF) {
			self->timer -= steps;
			if (self->timer < 0)
				self->timer = 0;
		}
	} else {
		(*self->state)(self);
		if (!(self->flags & 0x8000))
			return;
	}
	if (self->flags & 8) {
		Obj_UpdateBoxes(self);
		self->flags &= ~8;
	}
}
