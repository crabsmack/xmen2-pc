/* Anim_AdvanceByDistance - drive a distance-driven animation: add the distance moved to animDist
 * and take a step (Anim_Advance, Anim_ApplyStep(obj, 1)) once it reaches the script's step
 * distance either way.
 *
 * obj: the object (any Actor class).  dist: the distance moved (scripts without flags $80).
 * dx, dy: the movement in x and y (flags $80: dy with flags bit 4, else dx; their low words).
 * A step takes the step distance off animDist (adds it when animDist is not positive). Returns
 * Anim_ApplyStep's result, 0 with no animation, no step, or a one-shot script that ended. */
#include "xmen2.h"

char Anim_AdvanceByDistance(void *obj, short dist, long dx, long dy)
{
	register Actor *o = (Actor *)obj;

	if (!o->anim)
		return 0;
	{
		register short flags = ((AnimScript *)o->anim)->flags;

		if (flags & 0x80)
			o->animDist += flags & 0x10 ? dy : dx;
		else
			o->animDist += dist;
	}
	{
		register short step = ((AnimScript *)o->anim)->delay;

		if (step <= o->animDist || o->animDist <= -step) {
			if (o->animDist > 0)
				o->animDist -= step;
			else
				o->animDist += step;
			Anim_Advance(o);
			if (!o->anim)
				return 0;
			return Anim_ApplyStep(o, 1);
		}
	}
	return 0;
}
