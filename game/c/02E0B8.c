/* ExodusBlast_LaunchRight - EXODUSBLAST kind 1: fly right at $32C (no gravity), the trail drawn
 * 32 px apart to the left, until the right wall (ExodusBlast_StateFlyRight).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_LaunchRight(register ExodusBlast *self)
{
	self->vx = 0x32C;
	self->vy = 0;
	self->ay = 0;
	self->trailDx = -0x20;
	SET_STATE(self, ExodusBlast_StateFlyRight);
	return self->flags & 8;
}
