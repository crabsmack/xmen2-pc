/* Obj_AttachChild - attach an object to a parent, as its first child.
 *
 * p: the parent.  c: the object to attach (both any Actor class).
 * The child is detached from anything first (Obj_Detach), rides (flags $800) with its timer
 * stopped, and goes at the head of the parent's chain: its next sibling is the old first child
 * (whose previous link spr.unk_0F becomes the child), its parent link spr.unk_0C the parent. */
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
	child->spr.nextSibling = parent->spr.firstChild;
	parent->spr.firstChild = child->index;
	child->spr.unk_0C = parent->index;
	parent = child->spr.nextSibling == 0xFF ? 0 : g_ObjTable[child->spr.nextSibling];
	if (parent)
		parent->spr.unk_0F = child->index;
}
