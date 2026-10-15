/* Obj_InheritLayer - give an object its parent's draw layer, and pass it on.
 *
 * parent: the object whose layer it takes.  o: the object.
 * The objects o carries get it too (Obj_ForEachChild with this function), and so do the floor
 * segments it owns (Seg_ForEachOfObj, Seg_SetLayerFromObj). */
#include "xmen2.h"

void Obj_InheritLayer(Actor *parent, register Actor *o)
{
	o->layer = parent->layer;
	if (o->spr.firstChild != 0xFF)
		Obj_ForEachChild(o, Obj_InheritLayer);
	if (o->segIndex != 0xFF)
		Seg_ForEachOfObj(o, Seg_SetLayerFromObj);
}
