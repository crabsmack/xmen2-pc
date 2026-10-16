/* CoreElevator_WaitAtTop - COREELEVATOR state at the upper stop: when a player is below it, make
 * one trip down (trips 1: CoreElevator_WaitAtBottom brings it back) through CoreElevator_StartDown;
 * else look again in 255 frames.
 *
 * self: the lift. Returns flags & 8. */
#include "xmen2.h"

short CoreElevator_WaitAtTop(register CoreElevator *self)
{
	if (CoreElevator_PlayerBelow(self, (Actor *)&g_Player1)
	    || CoreElevator_PlayerBelow(self, (Actor *)&g_Player2)) {
		self->trips = 1;
		SET_STATE(self, CoreElevator_StartDown);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, CoreElevator_WaitAtTop, 0xFF);
	return self->flags & 8;
}
