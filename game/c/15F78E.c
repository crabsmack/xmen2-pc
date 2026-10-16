/* Anim_ApplyStep - carry out an object's current animation step: sound steps are played and passed
 * (Anim_Advance), then the frame step is shown (SetFrame, class slot +$56) with its flips.
 *
 * obj: the object (any Actor class).  flip: apply the step's own flip bits (scripts with flags
 * bit 2: step flags $80 toggles the h-flip, $40 the v-flip).
 * Scripts with flags bit 3 face the way the object moves (vx < 0 left, > 0 right). The sprite's
 * h/v-flip bits are set from the result. Returns flip; 0 when a sound step ended the script. */
#include "xmen2.h"

/* The object's current step, addressed as the original did: the script's address as a number
   plus step * 2, the step at +6 of that. PC: the same step. */
#ifdef XMEN2_MPW
#define CUR_STEP(o)	(&((AnimScript *)((long)(o)->anim + ((o)->animStep << 1)))->step[0])
#else
#define CUR_STEP(o)	(&((AnimScript *)(o)->anim)->step[(o)->animStep])
#endif

char Anim_ApplyStep(void *obj, char flip)
{
	register Actor *o = (Actor *)obj;
	register short flags = ((AnimScript *)o->anim)->flags;
	short frame;

	while (CUR_STEP(o)->flags & 0x20) {
		register short snd = CUR_STEP(o)->value;

		Snd_Stop(snd);
		Snd_Play(snd);
		Anim_Advance(o);
		if (!o->anim)
			return 0;
	}
	{
		register char hflip = o->spr.flags & 0x10;
		register char vflip = o->spr.flags & 8;

		frame = CUR_STEP(o)->value;
		if (flags & 4) {
			if (flip) {
				register short stepFlags = CUR_STEP(o)->flags;

				if (stepFlags & 0x80)
					hflip = !hflip;
				if (stepFlags & 0x40)
					vflip = !vflip;
			}
		} else if (flags & 8) {
			if (o->vx < 0)
				hflip = 1;
			if (o->vx > 0)
				hflip = 0;
		}
		(*o->cls->method[20])(o, frame);		/* SetFrame */
		o->spr.flags &= ~0x18;
		o->spr.flags |= (hflip ? 0x10 : 0) | (vflip ? 8 : 0);
	}
	return flip;
}
