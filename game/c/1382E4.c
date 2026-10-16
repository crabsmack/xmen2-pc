/* LaserCannon_StateIdle - LASERCANNON state: the rest frame (0), then LaserCannon_StatePickTarget
 * from the next update.
 *
 * self: the cannon. Returns flags & 8. */
#include "xmen2.h"

short LaserCannon_StateIdle(LaserCannon *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	SET_STATE(self, (StateFn)LaserCannon_StatePickTarget);
	return self->flags & 8;
}
