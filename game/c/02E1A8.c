/* ExodusBlast_LaunchDown - EXODUSBLAST kind 4: fly straight down at $386 (no gravity), the trail
 * drawn 32 px apart above it, until the floor (ExodusBlast_StateFlyDown).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_LaunchDown(register ExodusBlast *self)
{
	self->vx = 0;
	self->vy = 0x386;
	self->ay = 0;
	self->trailDy = -0x20;
	SET_STATE(self, ExodusBlast_StateFlyDown);
	return self->flags & 8;
}
