/* FloatBubble_StatePop - FLOATBUBBLE hurt state (+$60): pop - frame $F, sound $BA, then frames
 * $10 and $11 (FloatBubble_StatePop2) and gone, 6 frames each.
 *
 * self: the bubble (never runs in the corpus: its TakeDamage bursts it instead).
 * Returns self->flags & 8. */
#include "xmen2.h"

short FloatBubble_StatePop(FloatBubble *self)
{
	(*self->cls->method[20])(self, 0xF);		/* +$56 SetFrame */
	Snd_Stop(0xBA);
	Snd_Play(0xBA);
	SET_STATE_WAIT(self, FloatBubble_StatePop2, 6);
	return self->flags & 8;
}
