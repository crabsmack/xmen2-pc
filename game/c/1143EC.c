/* XNpc_StateStun - XNPC state: stunned: shake (XNpc_ShakeStep) while the shake counter runs down
 * (2 a frame in two-player step mode, never below 0), then the hurt is over (EndHurt).
 *
 * self: the object.
 * Returns flags bit 3 while shaking; nothing in particular after EndHurt. */
#include "xmen2.h"

short XNpc_StateStun(XNpc *self)
{
	if (self->shake > 0) {
		XNpc_ShakeStep(self);
		self->shake -= self->shake > 1 ? (g_TwoPlayerStep ? 2 : 1) : (self->shake == 1 ? 1 : 0);
		SET_STATE(self, XNpc_StateStun);
		return self->flags & 8;
	}
	(*self->cls->method[28])(self);	/* +$76 EndHurt */
	NO_RETURN_VALUE
}
