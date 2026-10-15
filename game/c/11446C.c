/* XNpc_StateKnockStart - XNPC state (XNpc_HurtReact, hit types 9 and 10): knocked back: not
 * hittable, inactive (flags6 |= $5000), shake for 12 frames first (XNpc_StateKnockShake from the
 * next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short XNpc_StateKnockStart(XNpc *self)
{
	self->flags6 |= 0x5000;
	self->shake = 12;
	SET_STATE(self, XNpc_StateKnockShake);
	return self->flags & 8;
}
