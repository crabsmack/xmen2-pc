/* FlyBrood_StateHoverFacePlayer - FLYBROOD state after FlyBrood_ActHover90: face the nearest
 * player; once the countdown has run out, the next step now.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void FlyBrood_StateHoverFacePlayer(FlyBrood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	if (self->countdown <= 0) {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
