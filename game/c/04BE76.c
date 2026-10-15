/* Apocalypse_ActGoNearPlayer - APOCALYPSE script action (also the retry of
 * Apocalypse_StateApproach): target the nearest player and make a damped move (approach shift)
 * to 130 px beside it, on this side, at height 140, facing it with frame 0; then approach it
 * (Apocalypse_StateApproach, run now).
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_ActGoNearPlayer(register Apocalypse *self)
{
	register Actor *p = self->target = (Actor *)Player_Nearest((Actor *)self);
	register long px = p->x;
	register long dx = 0x8200;
	register long x;
	register long y = 0x8C00;

	if (self->x < px) {
		self->spr.flags &= ~0x10;
		dx = -dx;
	} else
		self->spr.flags |= 0x10;
	x = px + dx;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	Apocalypse_MoveTo(self, x, y, self->goShift);
	self->state = Apocalypse_StateApproach;	/* and run it now */
	Apocalypse_StateApproach(self);
end:	/* (here MPW C pops all the arguments, as the ROM does) */
	;
}
