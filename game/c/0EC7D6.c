/* CoreElevator_SendUp - send the lift up (HOLOGRAM): CoreElevator_StartUp becomes its state and
 * default state, at once.
 *
 * self: the lift. */
#include "xmen2.h"

void CoreElevator_SendUp(register CoreElevator *self)
{
	self->state = self->defaultState = CoreElevator_StartUp;
	self->timer = 0;
}
