/* Obj_CountOfClass - how many objects of a class are present.
 *
 * cls: the class.
 * Counts the objects of g_ObjTable (players included) with flags bit $8000 whose class is cls. */
#include "xmen2.h"

short Obj_CountOfClass(register Class *cls)
{
	register short n;
	register short i;
	register Actor *o;

	n = 0;
	for (i = 0, o = g_ObjTable[0]; i < g_ObjCount; o = g_ObjTable[++i]) {
		if ((o->flags & 0x8000) && cls == o->cls)
			n++;
	}
	return n;
}
