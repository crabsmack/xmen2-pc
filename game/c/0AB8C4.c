/* FloatBubble_StateGrow4 - FLOATBUBBLE state: grow step 5 of 6: frame 4 for 10 frames
 * (FloatBubble_StateGrow5 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow4(FloatBubble *self)
{
	(*self->cls->method[20])(self, 4);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateGrow5, 10);
	return self->flags & 8;
}
