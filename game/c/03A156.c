/* BanimazaDoor_StateRaised - BANIMAZADOOR state: up: no longer solid (Draw stops moving its
 * segment and boxes) and no state update for $FF frames.
 *
 * self: the door. */
#include "xmen2.h"

void BanimazaDoor_StateRaised(register BanimazaDoor *self)
{
	self->solid = 0;
	self->timer = 0xFF;
}
