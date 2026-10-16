/* SwampFly_StateStart - SWAMPFLY's first state: frame 1, then pick a flight path from the next
 * update (SwampFly_StatePickPath).
 *
 * self: the fly. Returns self->flags & 8. */
#include "xmen2.h"

short SwampFly_StateStart(register SwampFly *self)
{
	(*self->cls->method[20])(self, 1);		/* +$56 SetFrame */
	SET_STATE(self, SwampFly_StatePickPath);
	return self->flags & 8;
}
