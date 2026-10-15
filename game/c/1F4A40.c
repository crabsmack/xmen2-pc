/* Obj_UpdateBoxes - recompute both of an object's boxes, and the floor segment it carries.
 *
 * obj: the object (any Actor class). Like Obj_UpdateBox for the body box, and the same for the second box
 * (box2*) from the frame's second offsets. An object that owns floor segments (flags6 bit
 * $200, segIndex not $FF) gets its first segment set to its body box's top edge (one pixel
 * down) and the length recomputed (Seg_ComputeLength). Hand-written 68000. */
#include "xmen2.h"

void Obj_UpdateBoxes(void *obj)
{
	Actor *o = (Actor *)obj;
	short x = (short)(o->x >> 8);
	short y = (short)(o->y >> 8);
	SprDef *def = (SprDef *)o->spr.def;
	SprFrame *f;
	Seg *s;

#ifndef XMEN2_MPW
	if (!def)	/* no definition: the 68000 reads the vector table at $24 on */
		def = (SprDef *)Vectors;
#endif
	f = def->frame;
	if (o->spr.frame != 0xFF)
		f += o->spr.frame;
	if (o->spr.flags & 0x10) {
		o->boxLeft = x - f->boxRight;
		o->boxRight = x - f->boxLeft;
		o->box2Left = x - f->box2Right;
		o->box2Right = x - f->box2Left;
	} else {
		o->boxLeft = x + f->boxLeft;
		o->boxRight = x + f->boxRight;
		o->box2Left = x + f->box2Left;
		o->box2Right = x + f->box2Right;
	}
	if (o->spr.flags & 0x08) {
		o->boxBottom = y - f->boxTop;
		o->boxTop = y - f->boxBottom;
		o->box2Bottom = y - f->box2Top;
		o->box2Top = y - f->box2Bottom;
	} else {
		o->boxBottom = y + f->boxBottom;
		o->boxTop = y + f->boxTop;
		o->box2Bottom = y + f->box2Bottom;
		o->box2Top = y + f->box2Top;
	}
	if ((o->flags6 & 0x200) && o->segIndex != 0xFF) {
		s = &g_Segs[o->segIndex];
		s->x1 = o->boxLeft;
		s->y1 = o->boxTop + 1;
		s->x2 = o->boxRight;
		s->y2 = o->boxBottom;
		Seg_ComputeLength(s);
	}
}
