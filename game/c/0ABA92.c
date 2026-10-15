/* FloatBubble_StatePop2 - FLOATBUBBLE state: popping: frame $10 for 6 frames
 * (FloatBubble_StatePop3 next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StatePop2(FloatBubble *self)
{
	(*self->cls->method[20])(self, 0x10);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, FloatBubble_StatePop3, 6);
	return self->flags & 8;
}
