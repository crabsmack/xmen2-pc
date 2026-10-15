/* Seg_MoveAlongSteps - move an object n times by its x speed along its floor segments.
 *
 * obj: the walker (any Actor class); n: the steps. Seg_MoveAlong(obj, vx) n times. */
#include "xmen2.h"

void Seg_MoveAlongSteps(void *obj, register short n)
{
	register Actor *o = (Actor *)obj;

	while (n--)
		Seg_MoveAlong(o, o->vx);
}
