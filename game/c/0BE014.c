/* Barbarian_StateLeapFall - BARBARIAN leaping, at the top: once it falls (vy > 0), frame $14 and
 * go on with Barbarian_StateLeapDrop.
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing before. */
#include "xmen2.h"

short Barbarian_StateLeapFall(Barbarian *self)
{
	if (self->vy > 0) {
		(*self->cls->method[20])(self, 0x14);		/* +$56 SetFrame */
		SET_STATE(self, Barbarian_StateLeapDrop);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
