/* LaserCannon_StateAim - LASERCANNON state before each shot: the aim frame (4), then
 * LaserCannon_StateFire in 2 frames.
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateAim(LaserCannon *self)
{
	(*self->cls->method[20])(self, 4);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateFire, 2);
	return self->flags & 8;
}
