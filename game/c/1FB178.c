/* Obj_ShiftTree - move an object, and everything attached to it, by the shift delta.
 *
 * parent: unused (the Obj_ForEachChild callback form).  o: the object.
 * x/y += g_ShiftDX/g_ShiftDY pixels (set by Spr_PlaceAttached), boxes recomputed, then the same
 * for the objects it carries. */
#include "xmen2.h"

void Obj_ShiftTree(Actor *parent, register Actor *o)
{
	o->x += g_ShiftDX << 8;
	o->y += g_ShiftDY << 8;
	Obj_UpdateBoxes(o);
	if (o->spr.firstChild != 0xFF)
		Obj_ForEachChild(o, Obj_ShiftTree);
}
