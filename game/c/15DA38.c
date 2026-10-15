/* Actor_New - ACTOR New (class slot +$0E): make an object of a class.
 *
 * unused: ignored (0 from Actor_SpawnFromRec).  cls: the class.  x, y: position (8 fraction
 * bits).  pl: the placement entry (the Init's spawn parameter).
 * Takes a free object (Obj_FindFreeSlot), gives it the class, runs the class's Init (slot +$12)
 * and Activates it (+$3E) when it is inside the activation window (+$3A). Returns the object,
 * 0 when none was free.
 * MPW: the Init call reads o->cls, which MPW takes from the register holding cls (movea.l a3,a0);
 * cls is in a register because it is used twice: the dead store at the end stands for the
 * second use the original had. */
#include "xmen2.h"

Actor *Actor_New(long unused, register Class *cls, long x, long y, void *pl)
{
	register Actor *o;

	o = Obj_FindFreeSlot();
	if (o) {
		o->cls = cls;
		(*o->cls->method[3])(o, x, y, pl);
		if ((char)(*o->cls->method[13])(o))
			(*o->cls->method[14])(o);
	}
	if (0)
		o->cls = cls;
	return o;
}
