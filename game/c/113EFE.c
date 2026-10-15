/* XNpc_EndHurt - XNPC class slot +$76: the hurt reaction is over: hittable and active again
 * (flags6 bits $1000 / $4000 off), back to the state saved when it was hit, hurt flag (flags bit
 * 2) off, the state runs on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void XNpc_EndHurt(XNpc *self)
{
	self->flags6 &= ~0x1000;
	self->flags6 &= ~0x4000;
	if (self->hurtState) {
		self->state = self->hurtState;
		self->hurtState = 0;
	}
	self->flags &= ~4;
	self->timer = 0;
}
