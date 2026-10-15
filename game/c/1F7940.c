/* Seg_SetLayerFromObj - give a floor segment its object's draw layer.
 *
 * obj: the owner (any Actor class); seg: one of its segments. seg->layer = obj->layer
 * (Obj_InheritLayer runs it through Seg_ForEachOfObj). */
#include "xmen2.h"

void Seg_SetLayerFromObj(void *obj, Seg *seg)
{
	seg->layer = ((Actor *)obj)->layer;
}
