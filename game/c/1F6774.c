/* Obj_ForEachChildCall - call func(child) for each object attached to a parent.
 *
 * parent: the parent.  func: e.g. Obj_Detach (Actor_Kill drops what it carries).
 * Like Obj_ForEachChild without the parent argument; the next index is read before func runs,
 * so func may unlink the child. Never runs in the corpus. */
#include "xmen2.h"

void Obj_ForEachChildCall(Actor *parent, register ObjChildFn func)
{
	register short i;
	register Actor *child;

	for (i = parent->spr.firstChild; i != 0xFF; ) {
		child = i == 0xFF ? 0 : g_ObjTable[i];
		i = child->spr.nextSibling;
		(*func)(child);
	}
}
