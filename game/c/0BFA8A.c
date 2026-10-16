/* SwampBubbles_StateIdle - SWAMPBUBBLES state: no frame shown, wait the longest delay ($FF) and
 * stay in this state.
 *
 * self: the bubbles (state and default state from SwampBubbles_Init).
 * Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short SwampBubbles_StateIdle(SwampBubbles *self)
{
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	SET_STATE_WAIT(self, SwampBubbles_StateIdle, 0xFF);
	return self->flags & 8;
}
