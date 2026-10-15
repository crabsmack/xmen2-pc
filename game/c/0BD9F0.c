/* Barbarian_NotifySegmentGone - floor segment seg has gone (a trap door opened): every live
 * BARBARIAN standing on it, not in a leap (flags bit $1000) and not told yet, gets segGone = 1.
 *
 * seg: the segment (TrapDoor_StateOpen).
 * Walks the object table (g_ObjTable, g_ObjCount entries) for objects of class BARBARIAN. */
#include "xmen2.h"

void Barbarian_NotifySegmentGone(Seg *seg)
{
	register Barbarian *o;
	short i;

	for (i = 0, o = (Barbarian *)(Actor *)*g_ObjTable; i < g_ObjCount;
	     o = (Barbarian *)(Actor *)g_ObjTable[++i]) {
		if (IS_ALIVE(o) && o->cls == CLASS(Tbl_Barbarian_vtable) && o->seg == seg
		    && !(o->flags & 0x1000) && !o->segGone)
			o->segGone = 1;
	}
}
