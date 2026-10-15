/* XNpc_StateKnockBShake - XNPC state: knockback B: shake (XNpc_ShakeStep) while the shake
 * counter runs down (2 a frame in two-player step mode), then slide to the edge
 * (XNpc_StateKnockBSlide from the next update). Never runs in the corpus.
 *
 * self: the object.
 * The slide ends at knockToX = x + the knockback distance (pixels); its speed and deceleration
 * (XNpc_CalcKnockback) go in prevVx / prevVy. Returns flags bit 3. */
#include "xmen2.h"

short XNpc_StateKnockBShake(XNpc *self)
{
	s16 decel, speed;

	if (self->shake > 0) {
		XNpc_ShakeStep(self);
		self->shake -= self->shake > 1 ? (g_TwoPlayerStep ? 2 : 1) : (self->shake == 1 ? 1 : 0);
		SET_STATE(self, XNpc_StateKnockBShake);
		return self->flags & 8;
	}
	self->knockToX = (self->x >> 8) + self->knock;
	XNpc_CalcKnockback(self, self->knock, &speed, &decel);
	self->prevVx = speed;
	self->prevVy = decel;
	SET_STATE(self, XNpc_StateKnockBSlide);
	return self->flags & 8;
}
