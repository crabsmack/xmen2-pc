/* ExodusBlast_LaunchDownLeft - EXODUSBLAST kind 2: fly down-left (-$386, $3EA; no gravity), the
 * trail drawn 32 px apart up and to the right, until the floor (ExodusBlast_StateFlyDown).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_LaunchDownLeft(register ExodusBlast *self)
{
	self->vx = -0x386;
	self->vy = 0x3EA;
	self->ay = 0;
	self->trailDx = 0x20;
	self->trailDy = -0x20;
	SET_STATE(self, ExodusBlast_StateFlyDown);
	return self->flags & 8;
}
