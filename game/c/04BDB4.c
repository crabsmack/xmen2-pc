/* Apocalypse_ActGoCentre - APOCALYPSE script action (choice scripts): face the centre, frame 0, a
 * damped move to (256, 140) at its approach shift, then approach the player
 * (Apocalypse_StateApproach, run now).
 *
 * self: the boss (never runs in the corpus). */
#include "xmen2.h"

void Apocalypse_ActGoCentre(register Apocalypse *self)
{
	register long x = 0x10000;
	register long y = 0x8C00;

	if (self->x < x)
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	Apocalypse_MoveTo(self, x, y, self->goShift);
	self->state = Apocalypse_StateApproach;	/* and run it now */
	Apocalypse_StateApproach(self);
end:	/* (here MPW C pops all the arguments, as the ROM does) */
	;
}
