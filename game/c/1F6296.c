/* Obj_ChangeAnim - switch an object to an animation script unless it already plays it.
 *
 * obj: the object (any Actor class).  anim: the animation script.
 * Obj_SetAnim when anim isn't the script playing; the same script keeps running. */
#include "xmen2.h"

void Obj_ChangeAnim(void *obj, register void *anim)
{
	register Actor *o = (Actor *)obj;

	if (anim != o->anim)
		Obj_SetAnim(o, anim);
}
