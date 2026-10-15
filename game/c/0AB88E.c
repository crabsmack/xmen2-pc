/* FloatBubble_StateGrow3 - FLOATBUBBLE state: grow step 4 of 6: frame 3 for 10 frames
 * (FloatBubble_StateGrow4 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow3(FloatBubble *self)
{
	(*self->cls->method[20])(self, 3);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateGrow4, 10);
	return self->flags & 8;
}
