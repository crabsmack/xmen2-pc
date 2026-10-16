/* BrainChild_StateRise1 - BRAINCHILD state: rising; once slower than $180 up, frame 3 (slot
 * +$56) and go on rising (BrainChild_StateRise2).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateRise1(BrainChild *self)
{
	if (self->vy > -0x180) {
		(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
		SET_STATE(self, BrainChild_StateRise2);
		return self->flags & 8;
	}
	SET_STATE(self, BrainChild_StateRise1);
	return self->flags & 8;
}
