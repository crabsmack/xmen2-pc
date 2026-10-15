/* FloatBubble_StateRelease - FLOATBUBBLE state after growing: frame 8, let go of the surface line
 * (bubbleFlags bit 1 off), 10 px up, set off in its direction and wobble in 6 frames
 * (FloatBubble_StateWobble1).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateRelease(FloatBubble *self)
{
	(*self->cls->method[20])(self, 8);		/* +$56 SetFrame */
	self->bubbleFlags &= ~2;
	self->y -= 0xA00;
	FloatBubble_SetDirection(self);
	SET_STATE_WAIT(self, FloatBubble_StateWobble1, 6);
	return self->flags & 8;
}
