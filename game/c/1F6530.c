/* Obj_Detach - detach an object from the object it rides and from its siblings.
 *
 * obj: the object (any Actor class; 0: nothing happens).
 * Riding a parent (spr.parent, an object-table index, not $FF): the object keeps the combined
 * speed: a free-moving object (flags $1000) takes the parent's, one on a floor segment adds it
 * to its own made to follow the segment (Seg_AlignObjVelocity); a parent on a segment counts
 * with its own speed aligned likewise, its vx then put back. The object's timer runs again and
 * a parent whose first child it was gets the next one. Then it leaves its sibling chain (the
 * next's previous link spr.prevSibling, the previous one's next link), becomes free-moving (flags $800
 * off, $1000 on) on no segment, with no parent and no siblings. */
#include "xmen2.h"

void Obj_Detach(void *obj)
{
	register Actor *o = (Actor *)obj;
	register Actor *parent;
	register short i;

	if (!o)
		return;
	parent = o->spr.parent == 0xFF ? 0 : g_ObjTable[o->spr.parent];
	if (parent) {
		if (o->flags & 0x1000) {
			if (parent->flags & 0x1000) {
				o->vx = parent->vx;
				o->vy = parent->vy;
			} else {
				i = parent->vx;
				Seg_AlignObjVelocity(parent);
				o->vx = parent->vx;
				o->vy = parent->vy;
				parent->vx = i;
			}
		} else {
			Seg_AlignObjVelocity(o);
			if (parent->flags & 0x1000) {
				o->vx += parent->vx;
				o->vy += parent->vy;
			} else {
				i = parent->vx;
				Seg_AlignObjVelocity(parent);
				o->vx += parent->vx;
				o->vy += parent->vy;
				parent->vx = i;
			}
		}
		Timer_Start(o->timerObj);
		if (parent->spr.firstChild == o->index)
			parent->spr.firstChild = o->spr.nextSibling;
	}
	i = o->spr.nextSibling;
	if (i != 0xFF)
		(i == 0xFF ? 0 : g_ObjTable[i])->spr.prevSibling = o->spr.prevSibling;
	i = o->spr.prevSibling;
	if (i != 0xFF)
		(i == 0xFF ? 0 : g_ObjTable[i])->spr.nextSibling = o->spr.nextSibling;
	o->flags &= ~0x800;
	o->flags |= 0x1000;
	o->seg = 0;
	o->spr.parent = 0xFF;
	o->spr.nextSibling = 0xFF;
	o->spr.prevSibling = 0xFF;
}
