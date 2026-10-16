/* SavageMediumBio_StateReady - SAVAGEMEDIUMBIO state: it can be hit and hit (flags6 bits 1 and 0
 * off, into the enemy body and attack hit lists), no animation, frame 6, then its AI script from
 * the first step in 60 frames (no script: the default state).
 *
 * self: the object (after SavageMediumBio_StateAppear's animation). Returns nothing in
 * particular. */
#include "xmen2.h"

short SavageMediumBio_StateReady(register SavageMediumBio *self)
{
	self->flags6 &= ~2;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->flags6 &= ~1;
	self->attackNode = HitList_Add(self, &g_HitListEnemyAtk);
	self->anim = 0;
	(*self->cls->method[20])(self, 6);		/* +$56 SetFrame */
	AI_FIRST_STEP(self, self->ai, 0x3C);
	NO_RETURN_VALUE
}
