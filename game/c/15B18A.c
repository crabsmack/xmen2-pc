/* Spr_SetFromDef - point a sprite instance at the sprite definition loaded in slot n.
 *
 * spr: the sprite instance (LittleLeg_New).  n: the g_SpriteDefs slot SprDef_Add filled.
 * Like Obj_SetSpriteDef on a bare instance: definition, VRAM tile range and palette bits. */
#include "xmen2.h"

void Spr_SetFromDef(SprInst *spr, unsigned char n)
{
	register SprInst *d;

	d = &g_SpriteDefs[n];
	spr->def = d->def;
	spr->tileBase = d->tileBase;
	spr->tileCount = d->tileCount;
	spr->flags &= ~3;
	spr->flags |= 3 & d->flags;
}
