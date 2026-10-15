/* Gambit_StateAirThrowBrake - GAMBIT state after A is let go in the air: vy is eased by $100 a
 * frame into -$300..$100; then normal gravity and the air throw is aimed (Gambit_AirThrowAim,
 * now). Landed: aim on the ground instead (Gambit_GroundAim, now).
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_StateAirThrowBrake(register Gambit *self)
{
	register short vy = self->vy;

	if (!(self->flags & 0x1000))
		GOTO_STATE(self, (StateFn)Gambit_GroundAim);
	else if (vy < -0x300)
		self->vy += 0x100;
	else if (vy > 0x100)
		self->vy -= 0x100;
	else {
		self->ay = self->charRec->gravity;
		GOTO_STATE(self, (StateFn)Gambit_AirThrowAim);
	}
}
