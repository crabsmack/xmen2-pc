/* XNpc_StateStunStart - XNPC state (XNpc_HurtReact, hit types 11 and 12): stunned: not hittable,
 * inactive (flags6 |= $5000), shake for 12 frames (XNpc_StateStun from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short XNpc_StateStunStart(XNpc *self)
{
	self->flags6 |= 0x5000;
	self->shake = 12;
	SET_STATE(self, XNpc_StateStun);
	return self->flags & 8;
}
