/* Obj_StandOnSeg - put an object on a floor segment.
 *
 * obj: the object (any Actor class).  seg: the segment (0: nothing happens).
 * The object walks on it (seg; flags $1000 off: no longer free-moving) in its layer, at the
 * position along it nearest to where it is (Seg_ComputeObjPos), placed there
 * (Seg_ApplyObjPos) with its boxes recomputed. */
#include "xmen2.h"

void Obj_StandOnSeg(void *obj, register Seg *seg)
{
	register Actor *o = (Actor *)obj;

	if (seg) {
		o->seg = seg;
		o->flags &= ~0x1000;
		o->layer = seg->layer;
		Seg_ComputeObjPos(seg, o);
		Seg_ApplyObjPos(o);
		Obj_UpdateBoxes(o);
	}
}
