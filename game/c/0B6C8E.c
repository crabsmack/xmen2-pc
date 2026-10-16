/* BrainChild_StateRise2 - BRAINCHILD state: rising; once slower than $10E up, frame 4 (slot
 * +$56) at the top of the leap (BrainChild_StateApex).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateRise2(BrainChild *self)
{
	if (self->vy > -0x10E) {
		(*self->cls->method[20])(self, 4);	/* +$56 SetFrame */
		SET_STATE(self, BrainChild_StateApex);
		return self->flags & 8;
	}
	SET_STATE(self, BrainChild_StateRise2);
	return self->flags & 8;
}
