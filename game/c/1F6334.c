/* Obj_SetSpriteDef - give an object the sprite definition loaded in slot n of g_SpriteDefs.
 *
 * self: the object (37 callers, class Init methods).
 * n:    the slot SprDef_Add filled when the level loaded.
 * Copies the slot's definition pointer and VRAM tile range into the object's sprite
 * instance, and its palette bits (0-1) into the instance's flags. */
#include "xmen2.h"

void Obj_SetSpriteDef(Actor *self, unsigned char n)
{
	register SprInst *d;

	d = &g_SpriteDefs[n];
	self->spr.def = d->def;
	self->spr.tileBase = d->tileBase;
	self->spr.tileCount = d->tileCount;
	self->spr.flags = (self->spr.flags & ~3) | (d->flags & 3);
}
