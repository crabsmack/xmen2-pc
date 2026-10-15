/* XPlayer_HardLanding - XPLAYER class slot +$EA: land from a long fall (XPlayer_Land: fall speed
 * over 1099): the hard-landing frame, stopped, for the character's recovery time
 * (XPlayer_StateHardLanding).
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_HardLanding(XPlayer *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, self->charRec->hardLandFrame);	/* SetFrame */
	STOP_MOVING(self);
	self->countdown = self->charRec->hardLandTime;
	SET_STATE(self, (StateFn)XPlayer_StateHardLanding);
	return self->flags & 8;
}
