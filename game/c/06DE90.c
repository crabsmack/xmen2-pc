/* Brood_StateBreakOutRun - BROOD state: keep flashing while it runs; when the countdown runs out,
 * back to palette 1, stop (idle animation), reload the cooldown and go on with the next
 * AI-script step.
 *
 * self: the object. */
#include "xmen2.h"

void Brood_StateBreakOutRun(Brood *self)
{
	if (!--self->flashTimer) {
		short pal = (self->spr.flags & 3) ^ 2;

		self->spr.flags = self->spr.flags & ~3 | pal;
		self->flashTimer = 4;
	}
	if (self->countdown == 0) {
		self->spr.flags = self->spr.flags & ~3 | 1;
		self->ax = 0;
		self->vx = 0;
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
		self->cooldown = self->cooldownTime;
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
