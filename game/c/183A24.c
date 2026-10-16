/* Beast_RollProbeCb - the floor-contact hook (g_SegResolveHook) while Beast_RollPathClear probes
 * the roll's path: mark the contact instead of resolving it.
 *
 * seg: the segment touched (unused).  obj: the object (pushSeg = 1: something is in the way).
 * contact: the contact record of Obj_CollideSegs: its flags word gets $8000. */
#include "xmen2.h"

void Beast_RollProbeCb(Seg *seg, Actor *obj, s16 *contact)
{
	obj->pushSeg = 1;
	*contact |= 0x8000;
}
