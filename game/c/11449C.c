/* XNpc_StateKnockShake - XNPC state: knocked back: shake (XNpc_ShakeStep) while the shake
 * counter runs down (2 a frame in two-player step mode), then slide the knockback distance for
 * up to 16 frames (XNpc_StateKnockSlide from the next update).
 *
 * self: the object.
 * The slide's speed and deceleration (XNpc_CalcKnockback) go in prevVx / prevVy. Returns flags
 * bit 3. */
#include "xmen2.h"

short XNpc_StateKnockShake(XNpc *self)
{
	s16 decel, speed;

	if (self->shake > 0) {
		XNpc_ShakeStep(self);
		self->shake -= self->shake > 1 ? (g_TwoPlayerStep ? 2 : 1) : (self->shake == 1 ? 1 : 0);
		SET_STATE(self, XNpc_StateKnockShake);
		return self->flags & 8;
	}
	XNpc_CalcKnockback(self, self->knock, &speed, &decel);
	self->prevVx = speed;
	self->prevVy = decel;
	self->shake = 16;
	SET_STATE(self, XNpc_StateKnockSlide);
	return self->flags & 8;
}
