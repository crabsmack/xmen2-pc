/* Obj_AttachChild - attach an object to a parent, as its first child.
 *
 * p: the parent.  c: the object to attach (both any Actor class).
 * The child is detached from anything first (Obj_Detach), rides (flags $800) with its timer
 * stopped, and goes at the head of the parent's chain: its next sibling is the old first child
 * (whose previous link spr.prevSibling becomes the child), its parent link spr.parent the parent.
 * A null parent (a landing on a floor segment whose owner is $FF: Obj_CollideSegs) makes the
 * 68000 read the vector table ($3B, $15) and write to ROM, which does nothing. */
#include "xmen2.h"

void Obj_AttachChild(void *p, void *c)
{
	register Actor *parent = (Actor *)p;
	register Actor *child = (Actor *)c;
	Timer *t;

	Obj_Detach(child);
	child->flags |= 0x800;
	t = child->timerObj;
	if (t)
		Timer_Stop(t);
#ifndef XMEN2_MPW
	if (!parent) {
		parent = (Actor *)Vectors;
		child->spr.nextSibling = parent->spr.firstChild;
		child->spr.parent = parent->index;
	} else
#endif
	{
		child->spr.nextSibling = parent->spr.firstChild;
		parent->spr.firstChild = child->index;
		child->spr.parent = parent->index;
	}
	parent = child->spr.nextSibling == 0xFF ? 0 : g_ObjTable[child->spr.nextSibling];
	if (parent)
		parent->spr.prevSibling = child->index;
}
