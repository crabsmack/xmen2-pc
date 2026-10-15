/* XNpc_StateKnockBExpire - XNPC state: knockback B, off the edge: drift at the slide speed
 * (prevVx; twice a frame in two-player step mode) until the countdown (20 frames) runs out, then
 * blow up (Explode). Never runs in the corpus.
 *
 * self: the object.
 * Returns flags bit 3 while drifting; nothing in particular after Explode. */
#include "xmen2.h"

short XNpc_StateKnockBExpire(XNpc *self)
{
	if (self->countdown > 0) {
		self->x += self->prevVx;
		if (g_TwoPlayerStep)
			self->x += self->prevVx;
		SET_STATE(self, XNpc_StateKnockBExpire);
		return self->flags & 8;
	}
	(*self->cls->method[30])(self);	/* +$7E Explode */
	NO_RETURN_VALUE
}
