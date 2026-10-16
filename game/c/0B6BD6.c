/* BrainChild_StateLeap - BRAINCHILD state: leap out of the ship: no animation, frame 3 (slot
 * +$56), $200 forward (left with sprite flag $10, else right), $520 up, gravity $3C; then rise
 * (BrainChild_StateRise1).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateLeap(BrainChild *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
	self->vx = !(self->spr.flags & 0x10) ? 0x200 : -0x200;
	self->vy = -0x520;
	self->ay = 0x3C;
	SET_STATE(self, BrainChild_StateRise1);
	return self->flags & 8;
}
