/* ThronePhone_Init - THRONEPHONE Init (class slot +$12): the parent's (ACTOR) Init, then no
 * vertical speed or gravity, state and default state ThronePhone_StateInit, flags6 |= $5083,
 * its sprite definition (g_ThronePhoneSprDef, palette 0), no boss yet, mode 0 and both phases
 * 0, and the shape of mode 0 (its SetMode, class slot +$6E).
 *
 * self: the beam.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void ThronePhone_Init(register ThronePhone *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_ThronePhone_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = ThronePhone_StateInit;
	self->defaultState = ThronePhone_StateInit;
	self->flags6 |= 0x5083;
	Obj_SetSpriteDef((Actor *)self, g_ThronePhoneSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
	self->boss = 0;
	self->unread_90 = 0;
	self->mode = 0;
	self->angleX = 0;
	self->angleY = 0;
	(*self->cls->method[26])(self, self->mode);	/* +$6E ThronePhone_SetMode */
}
