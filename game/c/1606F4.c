/* Obj_GetLiftCarrier - the object an object rides when that one is a lift.
 *
 * obj: the object (a player: Cam_TrackPlayer).
 * Its parent (spr.unk_0C) is a lift when, free-moving (flags $1000), it moves more vertically
 * than horizontally (|vy| > |vx|), or, on a floor segment, the segment is a rail of type '$' or
 * '#'. Returns the parent then, else 0 (also with no parent). */
#include "xmen2.h"

Actor *Obj_GetLiftCarrier(void *obj)
{
	register Actor *o = (Actor *)obj;
	register Actor *c;
	short type;

	c = o->spr.unk_0C == 0xFF ? 0 : g_ObjTable[o->spr.unk_0C];
	if (!c)
		return 0;
	if (c->flags & 0x1000) {
		if (ABS(c->vy) > ABS(c->vx))
			return c;
		return 0;
	}
	{
		register Seg *s = c->seg;

		type = s->type;
		if (type == '$' || type == '#')
			return c;
		return 0;
	}
}
