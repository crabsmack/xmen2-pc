/* ICandy_SetAnim - start an animation on an ICANDY: from step 0, showing its first frame.
 *
 * obj:  the object (any ICANDY subclass).
 * anim: the animation script (AnimScript; steps of 2 bytes from +6, the frame in the second). */
#include "xmen2.h"

void ICandy_SetAnim(void *obj, void *anim)
{
	register ICandy *self = (ICandy *)obj;
	register short frame;

	self->anim = (u8 *)anim;
	self->animStep = 0;
	self->animTime = 0;
	frame = *(self->anim + (self->animStep << 1) + 7);
	self->frame = (u8)frame;
}
