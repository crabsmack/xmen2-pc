/* XPlayer_StartDeath - the player dies: the death sound, the hurt frame, then the blinking
 * (XPlayer_StateDeathBlink). Installed by XPlayer_TakeDamage when health reaches 0.
 *
 * self: the player.
 * px (+$10) becomes the blink's on-time, growing every 20 frames (stateTimer).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_StartDeath(XPlayer *self)
{
	Snd_Stop(0xD5);
	Snd_Stop(0x52);
	Snd_Play(0x52);
	(*self->cls->method[20])(self, self->charRec->hurtFrame);	/* SetFrame */
	self->countdown = 10;
	self->stateTimer = 20;
	self->px = 0;
	SET_STATE(self, (StateFn)XPlayer_StateDeathBlink);
	return self->flags & 8;
}
