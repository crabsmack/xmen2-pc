/* Barbarian_EndHurt - BARBARIAN EndHurt (class slot +$76): the parent's EndHurt, then flags6
 * bits $5000 back as they were when the hit came (hurtFlags6); if it was walking to a target, stop
 * and go for it again in 5 frames (Barbarian_StateEngage).
 *
 * self: the object. */
#include "xmen2.h"

void Barbarian_EndHurt(Barbarian *self)
{
	(*CLASS(Tbl_Barbarian_vtable)->parent->method[28])(self);	/* BIONPC EndHurt */
	self->flags6 |= self->hurtFlags6 & 0x5000;
	if (self->walking) {
		self->state = Barbarian_StateEngage;
		self->timer = 5;
		self->ax = 0;
		self->vx = 0;
	}
}
