/* Obj_ForEachChild - call func(parent, child) for each object attached to a parent.
 *
 * obj: the parent (any Actor class).  func: e.g. Obj_CarryChild (Actor_Update), recursively.
 * The attached objects are a chain of object-table indexes: the parent's spr.firstChild, then
 * each one's spr.nextSibling, $FF ending it. The next index is read before func runs. */
#include "xmen2.h"

void Obj_ForEachChild(void *obj, ObjChildFn func)
{
	register Actor *parent = (Actor *)obj;
	register short i;
	register Actor *child;

	for (i = parent->spr.firstChild; i != 0xFF; ) {
		child = i == 0xFF ? 0 : g_ObjTable[i];
		i = child->spr.nextSibling;
		(*func)(parent, child);
	}
}
