/* Spr_PlaceAttached - place and draw the objects attached to a sprite.
 *
 * spr: the carrier's sprite instance (its firstChild chain; not empty).  x, y: the anchor's
 * screen position (Spr_EmitPreLinked / Spr_EmitStreamLinked: an anchor piece; Actor_DrawSprite: 0, 0).
 * A carried object (flags $1800) whose screen position (as Actor_Draw computes it, its frame's
 * box bottom above y: frame 0 when it shows none) isn't the anchor is moved there with what it
 * carries (g_ShiftDX/DY, Obj_ShiftTree). Every attached object then runs its DrawIfActive (class
 * slot +$1E). */
#include "xmen2.h"

void Spr_PlaceAttached(SprInst *spr, short x, short y)
{
	register Actor *o;
	register short fr;
	register short ay;
	register short cx;
	register short cy;
#ifndef XMEN2_MPW
	SprDef *def;
#endif

	o =spr->firstChild == 0xFF ? 0 : g_ObjTable[spr->firstChild];
	for (;;) {
		if ((o->flags & 0x1800) == 0x1800) {
			fr = o->spr.frame;
			if (fr == 0xFF)
				fr = 0;
#ifdef XMEN2_MPW
			ay = y - SPR_FRAME(o->spr.def, fr)->boxBottom;
#else
			def = (SprDef *)o->spr.def;
			if (!def)	/* no definition: the 68000 reads the vector table */
				def = (SprDef *)Vectors;
			ay = y - SPR_FRAME(def, fr)->boxBottom;
#endif
			cx = o->anim && (((AnimScript *)o->anim)->flags & 0x30) == 0x20
				? ((o->x - o->animDist) >> 8) - g_CameraX : (o->x >> 8) - g_CameraX;
			cy = o->anim && (((AnimScript *)o->anim)->flags & 0x30) == 0x30
				? ((o->y - o->animDist) >> 8) - g_CameraY : (o->y >> 8) - g_CameraY;
			if ((cx != x) | (ay != cy)) {
				g_ShiftDX = x - cx;
				g_ShiftDY = ay - cy;
				Obj_ShiftTree(0, o);
			}
		}
		(*o->cls->method[6])(o);		/* +$1E DrawIfActive */
		ay = o->spr.nextSibling;
		o = ay == 0xFF ? 0 : g_ObjTable[ay];
		if (ay == 0xFF)
			break;
	}
}
