/* YoyoAcolyte_StateThrowHigh - YOYOACOLYTE state: throw the yoyo in a high arc (class slot +$82,
 * YoyoAcolyte_ThrowYoyo kind 1), then wait for it (YoyoAcolyte_StateYoyoOut from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_StateThrowHigh(register YoyoAcolyte *self)
{
	(*self->cls->method[31])(self, 1);		/* +$82 ThrowYoyo */
	SET_STATE(self, (StateFn)YoyoAcolyte_StateYoyoOut);
	return self->flags & 8;
}
