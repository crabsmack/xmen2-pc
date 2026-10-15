/* FloatBubble_StateGrow5 - FLOATBUBBLE state: grow step 6 of 6: frame 5 for 10 frames, then it is released
 * (FloatBubble_StateRelease next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrow5(FloatBubble *self)
{
	(*self->cls->method[20])(self, 5);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StateRelease, 10);
	return self->flags & 8;
}
