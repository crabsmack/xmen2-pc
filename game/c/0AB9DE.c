/* FloatBubble_StateWobble2 - FLOATBUBBLE state: drifting, wobble frame 7 for 10 frames
 * (FloatBubble_StateWobble3 next).
 *
 * self: the bubble.
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateWobble2(FloatBubble *self)
{
	(*self->cls->method[20])(self, 7);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateWobble3, 10);
	return self->flags & 8;
}
