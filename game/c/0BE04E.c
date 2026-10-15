/* Barbarian_StateLeapDrop - BARBARIAN leaping, falling: once vy > $4B0, frame $15 and go on with
 * Barbarian_StateLeapNear.
 *
 * self: the object.
 * Returns self->flags & 8 then, nothing before. */
#include "xmen2.h"

short Barbarian_StateLeapDrop(Barbarian *self)
{
	if (self->vy > 0x4B0) {
		(*self->cls->method[20])(self, 0x15);		/* +$56 SetFrame */
		SET_STATE(self, Barbarian_StateLeapNear);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
