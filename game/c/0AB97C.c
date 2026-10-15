/* FloatBubble_StateStart - FLOATBUBBLE first state when it doesn't grow: set off in its direction
 * and wobble from the next frame (FloatBubble_StateWobble1).
 *
 * self: the bubble.
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateStart(FloatBubble *self)
{
	FloatBubble_SetDirection(self);
	SET_STATE(self, FloatBubble_StateWobble1);
	return self->flags & 8;
}
