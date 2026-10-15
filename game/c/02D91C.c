/* Exodus_Draw - EXODUS Draw (class slot +$1A): only the parent's (ACTOR) Draw.
 *
 * self: the boss.
 * It first works out its screen position and reserves 32 bytes of locals, then uses none of
 * it: what used them was compiled out. The unused variables below reproduce that frame; the
 * do/while (0) keeps the call's argument pop before returning, as the ROM does. */
#include "xmen2.h"

void Exodus_Draw(Exodus *self)
{
	HwSprite spr[4];
	register HwSprite *p = spr;
	register Exodus *o = self;
	register long sx, sy;

	sx = (o->x >> 8) - g_CameraX;
	sy = (o->y >> 8) - g_CameraY;
	if (0) {
		p->x = (short)sx;
		p->y = (short)sy;
	}
	do {
		(*CLASS(Tbl_Exodus_vtable)->parent->method[5])(o);	/* ACTOR Draw */
	} while (0);
}
