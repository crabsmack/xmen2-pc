/* CoreElevator_StateIdle - COREELEVATOR state (its first): stand still; its floor segment's layer
 * is 0. Runs itself again every 255 frames, until HOLOGRAM sends the lift (CoreElevator_SendDown /
 * _SendUp).
 *
 * self: the lift. Returns flags & 8. */
#include "xmen2.h"

short CoreElevator_StateIdle(register CoreElevator *self)
{
	g_Segs[self->segIndex].layer = 0;
	SET_STATE_WAIT(self, CoreElevator_StateIdle, 0xFF);
	return self->flags & 8;
}
