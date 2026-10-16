/* Elevator_Init - ELEVATOR's Init (class slot +$0A; SpaceA1_InstallHooks: 0, 0, $40): position,
 * speed (also the previous speed) and acceleration; going up, moving, the toggle free again.
 *
 * self: the lift.  pos: position (8 fraction bits).  speed, accel: speed and acceleration. */
#include "xmen2.h"

void Elevator_Init(register Elevator *self, long pos, register short speed, short accel)
{
	self->pos = pos;
	self->speed = speed;
	self->accel = accel;
	self->prevSpeed = speed;
	self->unk_0E = -1;
	self->up = 1;
	self->toggleDelay = 0;
	self->stopped = 0;
}
