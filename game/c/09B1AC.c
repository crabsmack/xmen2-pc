/* BroodClaw_Move - BROODCLAW Move (class slot +$4E): steps times, mirror the claw onto its pair
 * (motion, vx, mode, the swing offset from the other end, the other direction), reset the swing
 * length to $32 and run the motion function once (with 1).
 *
 * self:  the claw.
 * steps: Actor update steps (1, or 2 in two-player stepping); at least one round runs. */
#include "xmen2.h"

void BroodClaw_Move(register BroodClaw *self, short steps)
{
	register BroodClaw *pair;

	do {
		if (self->pair) {
			pair = self->pair;
			pair->motion = self->motion;
			pair->vx = self->vx;
			pair->mode = self->mode;
			pair->swingOffset = (pair->swingLen << 8) - self->swingOffset;
			pair->unk_8A[1] = self->unk_8A[1] ? 0 : 1;	/* swinging back */
		}
		self->swingLen = 0x32;
		if (self->motion)
			(*self->motion)(self, 1);
		steps--;
	} while (steps > 0);
}
