/* Zapper_Init - ZAPPER constructor (class slot +$12): the parent's (ACTOR) Init, then an animated
 * piece of scenery.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or null
 * (then a default one at x, y with every param 0, passed on to the parent too).
 * No vertical speed or gravity; flags6 |= $4082; sprite set g_ZapperSprDef, palette 1. Placement
 * param 0 picks the animation it loops: 0 (and any other value) Zapper_StateLoopAnim0, 1
 * Zapper_StateLoopAnim1, 2 Zapper_StateLoopAnim2 (the state and the default state); kept in
 * kind. */
#include "xmen2.h"

void Zapper_Init(register Zapper *self, register long x, register long y, register Placement *pl)
{
	Placement none;
	register short kind;

	if (!pl) {
		pl = &none;
		pl->x = x >> 8;
		pl->y = y >> 8;
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	(*CLASS(Tbl_Zapper_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->flags6 |= 0x4082;
	Obj_SetSpriteDef((Actor *)self, g_ZapperSprDef);
	self->spr.flags = self->spr.flags & ~3 | 1;
	kind = pl->param[0];
	switch (kind) {
	default:
		kind = 0;
	case 0:
		self->state = Zapper_StateLoopAnim0;
		self->defaultState = Zapper_StateLoopAnim0;
		break;
	case 1:
		self->state = Zapper_StateLoopAnim1;
		self->defaultState = Zapper_StateLoopAnim1;
		break;
	case 2:
		self->state = Zapper_StateLoopAnim2;
		self->defaultState = Zapper_StateLoopAnim2;
	}
	self->kind = kind;
}
