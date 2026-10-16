/* ICandy_SetSpriteDef - give an ICANDY the sprite definition of slot n of g_SpriteDefs (SprDef_Add):
 * its definition, VRAM tiles and palette.
 *
 * self: the object (any ICANDY subclass).  n: the g_SpriteDefs slot. */
#include "xmen2.h"

void ICandy_SetSpriteDef(void *obj, unsigned char n)
{
	ICandy *self = (ICandy *)obj;
	register SprInst *def = &g_SpriteDefs[n];

	self->sprDef = def->def;
	self->tileBase = def->tileBase;
	self->tileCount = def->tileCount;
	self->sprFlags &= ~3;
	self->sprFlags |= def->flags & 3;
}
