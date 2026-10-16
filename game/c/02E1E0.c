/* ExodusBlast_LaunchDropA - EXODUSBLAST kind 5 (swoop A's blast): drop straight down at $400 (no
 * gravity), the trail drawn 20 px apart above it, until the floor (ExodusBlast_StateFlyDown).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_LaunchDropA(register ExodusBlast *self)
{
	self->vx = 0;
	self->vy = 0x400;
	self->ay = 0;
	self->trailDy = -0x14;
	SET_STATE(self, ExodusBlast_StateFlyDown);
	return self->flags & 8;
}
