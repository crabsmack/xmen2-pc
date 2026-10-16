/* SwampPlatform_Init - SWAMPPLATFORM constructor (class slot +$12): the parent's (ACTOR) Init, then
 * a platform bobbing about its placement's y.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or 0.
 * flags6 |= $283; home height py = the placement's y (read before the test for no placement:
 * without one, the vector table's word at 4); draw layer 0; sprite set g_SwampPlatformSprDef,
 * palette 3, tile = its first VRAM tile | palette 3; no frame shown ($FF); from the placement:
 * bob phase (param 0) and height shift (param 1); state (and default state)
 * SwampPlatform_StateIdle.
 * (The palette store is a do/while (0), as a macro would be: MPW C pops the two calls' arguments
 * at its head, as the ROM does.) */
#include "xmen2.h"

void SwampPlatform_Init(register SwampPlatform *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_SwampPlatform_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x283;
#ifdef XMEN2_MPW
	self->py = pl->y;
#else
	self->py = (pl ? pl : (Placement *)Vectors)->y;	/* no placement: the 68000 reads the vector table */
#endif
	self->layer = 0;
	Obj_SetSpriteDef((Actor *)self, g_SwampPlatformSprDef);
	self->tile = g_SpriteDefs[g_SwampPlatformSprDef].tileBase | 0x6000;
	do {
		self->spr.flags = self->spr.flags & ~3 | 3;
	} while (0);
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	if (pl) {
		self->phase = pl->param[0] ? pl->param[0] : 0;
		self->shift = pl->param[1] ? pl->param[1] : 0;
	}
	self->unk_9A = 0;
	self->state = self->defaultState = SwampPlatform_StateIdle;
}
