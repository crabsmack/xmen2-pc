/* Barbarian_StateLeapNear - BARBARIAN leaping, falling: once less than 10 px above targetY (or
 * below it), go on with Barbarian_StateLeapLand.
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing before. */
#include "xmen2.h"

short Barbarian_StateLeapNear(Barbarian *self)
{
	if (self->targetY - (self->y >> 8) < 10) {
		SET_STATE(self, Barbarian_StateLeapLand);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
