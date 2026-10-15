/* FloatBubble_StatePopEnd - FLOATBUBBLE state at the end of the pop: remove itself.
 *
 * self: the bubble (never runs in the corpus). */
#include "xmen2.h"

void FloatBubble_StatePopEnd(FloatBubble *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
