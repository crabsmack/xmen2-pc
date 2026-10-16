/* MaintRobot_StatePatrol - MAINTROBOT state (patrol mode): drive back and forth, reversing when
 * blocked (flags bit $100, cleared), until its countdown runs out; then keep the velocity in
 * patrolVx and stop to fire (MaintRobot_StateStop from the next update).
 *
 * self: the object.
 * Returns flags bit 3 when it stops; no value while patrolling. */
#include "xmen2.h"

short MaintRobot_StatePatrol(register MaintRobot *self)
{
	if (self->countdown <= 0) {
		self->patrolVx = self->vx;
		SET_STATE(self, MaintRobot_StateStop);
		return self->flags & 8;
	}
	if (self->flags & 0x100) {
		self->flags &= ~0x100;
		self->vx = -self->vx;
	}
	NO_RETURN_VALUE
}
