/* ScalingActor_Activate - SCALINGACTOR class slot +$3E: the parent's (ACTOR) Activate, then its
 * sprite streams into its slot's VRAM tiles (the big sprite's buffer: tileBase, tiles; streamed
 * bit set), no frame shown yet.
 *
 * self: the portrait. */
#include "xmen2.h"

void ScalingActor_Activate(register ScalingActor *self)
{
	(*CLASS(Tbl_ScalingActor_vtable)->parent->method[14])(self);	/* ACTOR Activate */
	self->spr.tileBase = self->slot->tileBase;
	self->spr.tileCount = self->slot->tiles;
	self->spr.flags |= 0x80;
	self->spr.frame = 0xFF;
}
