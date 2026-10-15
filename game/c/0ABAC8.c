/* FloatBubble_StatePop3 - FLOATBUBBLE state: popping: frame $11 for 6 frames, then it is removed
 * (FloatBubble_StatePopEnd next).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StatePop3(FloatBubble *self)
{
	(*self->cls->method[20])(self, 0x11);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, (StateFn)FloatBubble_StatePopEnd, 6);
	return self->flags & 8;
}
