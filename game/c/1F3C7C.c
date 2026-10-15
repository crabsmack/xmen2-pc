/* XPlayer_StateHurt - the player's hurt state (hurtState, +$60): the hurt frame for the character's
 * hurt time, then XPlayer_StateHurtRecover.
 *
 * self: the player.
 * Ends a crouch attack (playerFlags $200000) and $400000. Every character's Init installs it.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_StateHurt(XPlayer *self)
{
	self->playerFlags &= ~0x200000;
	self->playerFlags &= ~0x400000;
	(*self->cls->method[20])(self, self->charRec->hurtFrame);	/* SetFrame */
	self->countdown = self->charRec->hurtTime;
	SET_STATE(self, (StateFn)XPlayer_StateHurtRecover);
	return self->flags & 8;
}
