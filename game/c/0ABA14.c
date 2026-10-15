/* FloatBubble_StateWobble3 - FLOATBUBBLE state: drifting, wobble frame 8 for 10 frames, then back to frame 6
 * (FloatBubble_StateWobble1 next).
 *
 * self: the bubble.
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateWobble3(FloatBubble *self)
{
	(*self->cls->method[20])(self, 8);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateWobble1, 10);
	return self->flags & 8;
}
