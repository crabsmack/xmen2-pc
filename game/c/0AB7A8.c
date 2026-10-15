/* FloatBubble_StateGrowStart - FLOATBUBBLE first state when it grows in place (bubbleFlags bit 0):
 * stop, sound $B9, start growing in 10 frames (FloatBubble_StateGrow0).
 *
 * self: the bubble (never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StateGrowStart(FloatBubble *self)
{
	STOP_MOVING(self);
	Snd_Stop(0xB9);
	Snd_Play(0xB9);
	SET_STATE_WAIT(self, FloatBubble_StateGrow0, 10);
	return self->flags & 8;
}
