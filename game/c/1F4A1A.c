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

#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2)	/* phase 7: the entry past the table is player 3 (Obj_AllocPool): not a free object */
		k--;
#endif
	do {
		o = *p++;
		if (o->flags == 0) {
#if defined(XMEN2_NATIVE)
			if (g_opt.players > 2)
				P34_TagNew(o);	/* phase 7: made by a player 3-4's code: its */
#endif
			return o;
		}
	} while (k-- != 0);
	return 0;
}
