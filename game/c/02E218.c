/* ExodusBlast_LaunchDropB - EXODUSBLAST kind 6 (swoop B's blast): the same drop as kind 5
 * (ExodusBlast_LaunchDropA): straight down at $400, trail 20 px apart, until the floor.
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_LaunchDropB(register ExodusBlast *self)
{
	self->vx = 0;
	self->vy = 0x400;
	self->ay = 0;
	self->trailDy = -0x14;
	SET_STATE(self, ExodusBlast_StateFlyDown);
	return self->flags & 8;
}
