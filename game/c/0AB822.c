/* FloatBubble_StateGrow1 - FLOATBUBBLE state: grow step 2 of 6: frame 1 for 10 frames
 * (FloatBubble_StateGrow2 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow1(FloatBubble *self)
{
	(*self->cls->method[20])(self, 1);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateGrow2, 10);
	return self->flags & 8;
}
