/* Elevator_Toggle - ELEVATOR's class slot +$12 (SpaceA1_SwitchTileHit, Deathbird): unless the
 * toggle delay runs, reverse the lift: 26 frames of delay, the other direction (acceleration $40
 * up, -$40 down), moving again if it was stopped.
 *
 * self: the lift. */
#include "xmen2.h"

void Elevator_Toggle(register Elevator *self)
{
	if (self->toggleDelay <= 0) {
		self->toggleDelay = 26;
		self->up = !self->up;
		if (self->stopped)
			self->stopped = 0;
		if (self->up)
			self->accel = 0x40;
		else
			self->accel = -0x40;
	}
}
