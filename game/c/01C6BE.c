/* YoyoAcolyte_ActIdle - YOYOACOLYTE script action: idle facing the nearest live player for
 * idleTime frames (YoyoAcolyte_StateIdleNext from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_ActIdle(register YoyoAcolyte *self)
{
	self->countdown = self->idleTime;
	SET_STATE(self, (StateFn)YoyoAcolyte_StateIdleNext);
	return self->flags & 8;
}
