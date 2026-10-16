/* Anim_Advance - move an object's animation on to its next step; past the last one it starts over,
 * or ends for a one-shot script.
 *
 * obj: the object (any Actor class).
 * At the end (step >= the script's count) the step goes back to 0; a one-shot script (flags bits
 * 0-1) is dropped (anim = 0), and with flags bit 1 the state's wait ends too (timer 0). */
#include "xmen2.h"

void Anim_Advance(void *obj)
{
	register Actor *o = (Actor *)obj;
	register u8 flags = ((AnimScript *)o->anim)->flags;

	o->animStep++;
	if (((AnimScript *)o->anim)->count <= o->animStep) {
		if (flags & 3) {
			if (flags & 2)
				o->timer = 0;
			o->anim = 0;
		}
		o->animStep = 0;
	}
}
