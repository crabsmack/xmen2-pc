/* HoloBolt_Update - HOLOBOLT Update (class slot +$4A; the parent's isn't called): a cut-down
 * ACTOR Update for a bolt that flies straight.
 *
 * self: the bolt.
 * Killed (class slot +$52) once its screen position is outside the deactivation window
 * (g_DeactWin*). Otherwise, unless frozen (flags6 $4000), it moves by its speed once per step
 * (2 in two-player step mode, else 1; no acceleration, no floor); its distance-driven animation
 * advances (a new frame sets flags 8); unless flags $200, the state runs when the timer is out
 * (else the timer runs down unless it is $FF or more), and a bolt gone after it is done; unless
 * flags $10, when it moved (flags 8) its boxes are recomputed and the flag cleared.
 * The start position is kept in shorts, so only its low 16 bits survive: the animation sees
 * distances off by the high bits and "moved" is set nearly always (kept: the original's
 * behaviour). */
#include "xmen2.h"

void HoloBolt_Update(register HoloBolt *self)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;
	register short y0;
	register short x0;

	if (sx > g_DeactWinRight || sx < g_DeactWinLeft || sy > g_DeactWinBottom || sy < g_DeactWinTop) {
		(*self->cls->method[19])(self);		/* +$52 Kill */
		return;
	}
	x0 = self->x;
	y0 = self->y;
	if (!(self->flags6 & 0x4000)) {
		do {
			self->x += self->vx;
			self->y += self->vy;
		} while (--steps);
	}
	steps = g_TwoPlayerStep ? 2 : 1;
	if ((char)Anim_AdvanceByDistance(self, steps, self->x - x0, self->y - y0))
		self->flags |= 8;
	if (!(self->flags & 0x200)) {
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
	}
	if (self->flags & 0x10)
		return;
	if (x0 != self->x || y0 != self->y)
		self->flags |= 8;
	if (self->flags & 8) {
		Obj_UpdateBoxes(self);
		self->flags &= ~8;
	}
}
