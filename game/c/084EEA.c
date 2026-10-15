/* SmallBrood_ActWaitCooldown - SMALLBROOD AI-script action: frame 0, and when the cooldown has run
 * out, reload it and go on with the next step (else nothing changes: the action runs again).
 *
 * self: the object.
 * The original has SmallBrood_StateWaitForPlayer's locals and code that was compiled out
 * after the SetFrame call (its frame: d4-d7/a3 saved, a 2-byte local); the if (0) block
 * stands for it and reproduces both. */
#include "xmen2.h"

void SmallBrood_ActWaitCooldown(SmallBrood *self)
{
	short left, top, right, bottom;
	long i;
	XPlayer *p;

	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	if (0) {
		p = &g_Player1;
		i = left = top = right = bottom = p->boxTop;
	}
	if (self->cooldown == 0) {
		self->cooldown = self->cooldownTime;
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
