/* CoreElevator_StartDown - COREELEVATOR state: start down at its speed (flags6 bit $4000, set
 * while stopped, off); next CoreElevator_MoveDown.
 *
 * self: the lift. Returns flags & 8. */
#include "xmen2.h"

short CoreElevator_StartDown(register CoreElevator *self)
{
	self->flags6 &= ~0x4000;
	self->vy = self->speed;
	SET_STATE(self, CoreElevator_MoveDown);
	return self->flags & 8;
}
