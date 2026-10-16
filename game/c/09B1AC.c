/* BroodClaw_Move - BROODCLAW Move (class slot +$4E): steps times, mirror the claw onto its pair
 * (motion, vx, mode, the swing offset from the other end, the other direction), reset the swing
 * length to $32 and run the motion function once (with 1).
 *
 * self:  the claw.
 * steps: Actor update steps (1, or 2 in two-player stepping); at least one round runs. */
#include "xmen2.h"

#define SWING_BACK(o)	((o)->unk_8A[1])	/* ACTOR +$8B, for a claw: 0 swinging out, 1 swinging back (BroodClaw_Swing; other classes use the byte differently) */

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
			SWING_BACK(pair) = SWING_BACK(self) ? 0 : 1;	/* the other direction */
		}
		self->swingLen = 0x32;
		if (self->motion)
			(*self->motion)(self, 1);
		steps--;
	} while (steps > 0);
}
