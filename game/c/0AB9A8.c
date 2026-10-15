/* FloatBubble_StateWobble1 - FLOATBUBBLE state: drifting, wobble frame 6 for 10 frames (the loop is frames 6, 7, 8)
 * (FloatBubble_StateWobble2 next).
 *
 * self: the bubble.
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateWobble1(FloatBubble *self)
{
	(*self->cls->method[20])(self, 6);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateWobble2, 10);
	return self->flags & 8;
}
