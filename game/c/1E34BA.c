/* Cyclops_StateAirBeamBrake - CYCLOPS A released in the air: bring vy into [-$300, $100], $100 a
 * frame, then fire the air beam (Cyclops_AirBeamAim, now, with the record's gravity).
 *
 * self: the player.
 * Landing first: aim on the ground (Cyclops_GroundAim, now). */
#include "xmen2.h"

void Cyclops_StateAirBeamBrake(register Cyclops *self)
{
	register short vy = self->vy;

	if (!(self->flags & 0x1000))
		GOTO_STATE(self, (StateFn)Cyclops_GroundAim);
	else if (vy < -0x300)
		self->vy += 0x100;
	else if (vy > 0x100)
		self->vy -= 0x100;
	else {
		self->ay = self->charRec->gravity;
		GOTO_STATE(self, (StateFn)Cyclops_AirBeamAim);
	}
}
