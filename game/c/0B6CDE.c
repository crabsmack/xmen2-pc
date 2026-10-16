/* BrainChild_StateApex - BRAINCHILD state: at the top of the leap; once falling faster than
 * $10E, frame 3 (slot +$56) and fall (BrainChild_StateFall).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateApex(BrainChild *self)
{
	if (self->vy > 0x10E) {
		(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
		SET_STATE(self, BrainChild_StateFall);
		return self->flags & 8;
	}
	SET_STATE(self, BrainChild_StateApex);
	return self->flags & 8;
}
