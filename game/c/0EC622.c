/* CoreElevator_WaitAtBottom - COREELEVATOR state at the lower stop: with trips 1 (sent down by
 * CoreElevator_WaitAtTop) go back up (CoreElevator_StartUp); with 0 (CoreElevator_SendDown) stay,
 * running this again every 255 frames.
 *
 * self: the lift. Returns flags & 8; nothing for other trips values. */
#include "xmen2.h"

short CoreElevator_WaitAtBottom(register CoreElevator *self)
{
	switch (self->trips) {
	case 0:
		SET_STATE_WAIT(self, CoreElevator_WaitAtBottom, 0xFF);
		return self->flags & 8;
	case 1:
		SET_STATE(self, CoreElevator_StartUp);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
