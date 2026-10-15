/* Obj_UpdateBox - recompute an object's body box from its position and current frame.
 *
 * obj: the object (any Actor class). The box (boxLeft/Top/Right/Bottom, pixels) is the pixel position plus the
 * frame record's offsets (frame $FF = the first record), mirrored when the sprite is
 * h-flipped (bit 4) or v-flipped (bit 3). Hand-written 68000; 19 callers. */
#include "xmen2.h"

void Obj_UpdateBox(void *obj)
{
	Actor *o = (Actor *)obj;
	short x = (short)(o->x >> 8);
	short y = (short)(o->y >> 8);
	SprDef *def = (SprDef *)o->spr.def;
	SprFrame *f;

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
	} else {
		o->boxLeft = x + f->boxLeft;
		o->boxRight = x + f->boxRight;
	}
	if (o->spr.flags & 0x08) {
		o->boxBottom = y - f->boxTop;
		o->boxTop = y - f->boxBottom;
	} else {
		o->boxBottom = y + f->boxBottom;
		o->boxTop = y + f->boxTop;
	}
}
