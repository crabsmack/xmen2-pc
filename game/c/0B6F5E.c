/* BrainChild_StateFire - BRAINCHILD state (after BrainChild_ActShoot): frame 0 (slot +$56), a
 * gauss bullet from $23 px in front of it and $1E px up (GaussBullet_Fire, flags $A | $8000 with
 * sprite flag 4 | $800 facing left), sound $EC restarted, then the next step of its AI script
 * in 10 frames.
 *
 * self: the object (never runs in the corpus).
 * The bullet's x, y start as the object's position and are then replaced by the offsets << 8
 * (the original's dead stores). MPW: the block's initializers give the ROM's long ?: (an
 * assignment would zero-extend a word). */
#include "xmen2.h"

void BrainChild_StateFire(BrainChild *self)
{
	(*self->cls->method[20])(self, 0);	/* +$56 SetFrame */
	{
		long bx = self->x;
		long by = self->y;
		short dx = 0x23;
		short dy = -0x1E;
		long flags = self->spr.flags & 4 ? 0x8000 : 0;

		if (self->spr.flags & 0x10) {
			dx = -dx;
			flags |= 0x800;
		}
		bx = dx << 8;
		by = dy << 8;
		flags |= 0xA;
		GaussBullet_Fire(bx, by, self, dx, dy, flags);
	}
	Snd_Stop(0xEC);
	Snd_Play(0xEC);
	AI_NEXT_STEP(self, self->ai, 10);
}
