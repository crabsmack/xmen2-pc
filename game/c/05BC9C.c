/* Boulder_Move - BOULDER Move (class slot +$4E; replaces ACTOR's): steps times, a whole boulder
 * moves by its speed, the speed by its acceleration, falling at most $800; a shattered one
 * (hitCooldown > 0) moves its three pieces instead (speed in quarter pixels, vy +2 a step).
 *
 * self: the boulder.  steps: steps to move (2 in two-player stepping).
 * Bug kept: a piece's vy is a byte, so its cap at $800 (back to 8) never applies and it wraps
 * at 127. */
#include "xmen2.h"

void Boulder_Move(register Boulder *self, register short steps)
{
	register BoulderPiece *p;
	register short i;

	while (steps) {
		steps--;
		if (self->hitCooldown > 0) {
			p = self->piece;
			for (i = 0; i < 3; i++, p++) {
				p->x += p->vx;
				p->y += p->vy;
				p->vy += 2;
				if (p->vy > 0x800)
					p->vy = 8;
			}
		} else {
			self->x += self->vx;
			self->y += self->vy;
			self->vx += self->ax;
			self->vy += self->ay;
			if (self->vy > 0x800)
				self->vy = 0x800;
		}
	}
}
