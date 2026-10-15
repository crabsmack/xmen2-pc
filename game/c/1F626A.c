/* Obj_SetAnim - start an animation script on an object from its first step.
 *
 * obj: the object (any Actor class).  anim: the animation script (an AnimScript header).
 * Step and distance 0, the first step shown (Anim_ApplyStep(obj, 1)), and the object redrawn
 * (flags |= 8). */
#include "xmen2.h"

void Obj_SetAnim(void *obj, void *anim)
{
	register Actor *o = (Actor *)obj;

	o->anim = anim;
	o->animStep = 0;
	o->animDist = 0;
	Anim_ApplyStep(o, 1);
	o->flags |= 8;
}
