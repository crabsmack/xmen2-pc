/* CoreElevator_SendDown - send the lift down to stay (HOLOGRAM): CoreElevator_StartDown becomes
 * its state and default state, trips 0 (it stays at the bottom), at once.
 *
 * self: the lift. */
#include "xmen2.h"

void CoreElevator_SendDown(register CoreElevator *self)
{
	self->state = self->defaultState = CoreElevator_StartDown;
	self->trips = 0;
	self->timer = 0;
}
