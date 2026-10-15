/* Obj_StandOnSegHere - stand an object on the floor segment that contains its position.
 *
 * obj: the object (any Actor class; MaintRobot_Init, Platform1_Init_Common).
 * Seg_FindContaining at its pixel position, any type (1001); Obj_StandOnSeg does nothing when
 * there is none. */
#include "xmen2.h"

void Obj_StandOnSegHere(void *obj)
{
	register Actor *o = (Actor *)obj;

	Obj_StandOnSeg(o, Seg_FindContaining(o->x >> 8, o->y >> 8, 1001));
}
