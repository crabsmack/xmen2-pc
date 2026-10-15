/* XNpc_StateKnockBSlide - XNPC state: knockback B: slide (prevVx, slowing by prevVy, twice a
 * frame in two-player step mode) until x passes knockToX the way it goes; then drift off the edge
 * for 20 frames (XNpc_StateKnockBExpire, now). Never runs in the corpus.
 *
 * self: the object.
 * Returns flags bit 3 while sliding, else what XNpc_StateKnockBExpire returns. */
#include "xmen2.h"

short XNpc_StateKnockBSlide(XNpc *self)
{
	if (self->knock > 0 && (self->x >> 8) > self->knockToX) {
		self->countdown = 20;
		self->state = XNpc_StateKnockBExpire;
		return XNpc_StateKnockBExpire(self);
	}
	if (self->knock < 0 && (self->x >> 8) < self->knockToX) {
		self->countdown = 20;
		self->state = XNpc_StateKnockBExpire;
		return XNpc_StateKnockBExpire(self);
	}
	self->x += self->prevVx;
	self->prevVx += self->prevVy;
	if (g_TwoPlayerStep) {
		self->x += self->prevVx;
		self->prevVx += self->prevVy;
	}
	SET_STATE(self, XNpc_StateKnockBSlide);
	return self->flags & 8;
}
