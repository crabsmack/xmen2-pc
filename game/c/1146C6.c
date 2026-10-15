/* XNpc_StateKnockBStart - XNPC state (XNpc_HurtReact: knocked toward an open edge of its range):
 * not hittable, inactive (flags6 |= $5000), shake for 12 frames first (XNpc_StateKnockBShake
 * from the next update). Never runs in the corpus.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short XNpc_StateKnockBStart(XNpc *self)
{
	self->flags6 |= 0x5000;
	self->shake = 12;
	SET_STATE(self, XNpc_StateKnockBShake);
	return self->flags & 8;
}
