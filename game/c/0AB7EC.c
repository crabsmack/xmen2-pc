/* FloatBubble_StateGrow0 - FLOATBUBBLE state: grow step 1 of 6: frame 0 for 10 frames
 * (FloatBubble_StateGrow1 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow0(FloatBubble *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateGrow1, 10);
	return self->flags & 8;
}
