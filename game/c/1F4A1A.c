/* Obj_FindFreeSlot - find an unused object in the object table, past the two player slots.
 *
 * Returns the first object from entry 2 whose flags word is 0, or 0 if there is none. Bug
 * kept: the dbra loop checks g_ObjCount - 1 entries from entry 2, one past the table's last
 * (g_ObjTable[g_ObjCount]). Hand-written 68000. */
#include "xmen2.h"

Actor *Obj_FindFreeSlot(void)
{
	PTR(Actor) *p = g_ObjTable + 2;
	unsigned short k = g_ObjCount - 2;
	Actor *o;

	do {
		o = *p++;
		if (o->flags == 0)
			return o;
	} while (k-- != 0);
	return 0;
}
