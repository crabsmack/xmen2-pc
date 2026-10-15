/* FloatBubble_StateGrow2 - FLOATBUBBLE state: grow step 3 of 6: frame 2 for 10 frames
 * (FloatBubble_StateGrow3 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow2(FloatBubble *self)
{
	(*self->cls->method[20])(self, 2);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateGrow3, 10);
	return self->flags & 8;
}
