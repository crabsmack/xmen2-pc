/* Barbarian_StateLeapRise - BARBARIAN leaping, rising: once the upward speed has dropped below
 * $708 (vy > -$708), frame $13 and go on with Barbarian_StateLeapFall.
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing before. */
#include "xmen2.h"

short Barbarian_StateLeapRise(Barbarian *self)
{
	if (self->vy > -0x708) {
		(*self->cls->method[20])(self, 0x13);		/* +$56 SetFrame */
		SET_STATE(self, Barbarian_StateLeapFall);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
