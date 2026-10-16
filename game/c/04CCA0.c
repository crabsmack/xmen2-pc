/* Apocalypse_Deactivate - APOCALYPSE Deactivate (class slot +$46): the parent's (ACTOR)
 * Deactivate, then give back the second sprite's VRAM tiles: every live object whose tiles lie
 * above them gets its frame invalidated (Spr_InvalidateFrame) and its tiles moved down by that
 * many, g_VramTileCursor too; the second sprite loses its 'uploaded' bit and +$0C/+$0D are $FF.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_Deactivate(register Apocalypse *self)
{
	register SprInst *spr2 = self->spr2;
	register short tileBase, tileCount, i;
	register Actor *o;

	(*CLASS(Tbl_Apocalypse_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
	spr2->flags &= ~0x20;
	tileBase = spr2->tileBase;
	tileCount = spr2->tileCount;
	for (i = 2, o = g_ObjTable[2]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o->spr.tileBase > tileBase) {
			Spr_InvalidateFrame(&o->spr);
			o->spr.tileBase -= tileCount;
		}
	}
	g_VramTileCursor -= tileCount;
	spr2->firstChild = 0xFF;
	spr2->parent = 0xFF;
}
