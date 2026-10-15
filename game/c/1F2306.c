/* XPlayer_Jump - XPLAYER class slot +$82 (C on the ground): leave the floor with the character's
 * jump speed, gravity and maximum speed in the air, and its jump sound ($FF: none).
 *
 * self: the player. playerFlags 8: it jumped; the animation stops. */
#include "xmen2.h"

void XPlayer_Jump(XPlayer *self)
{
	Obj_Detach(self);
	self->playerFlags |= 8;
	self->anim = 0;
	if (self->charRec->jumpSound != 0xFF) {
		Snd_Stop(self->charRec->jumpSound);
		Snd_Play(self->charRec->jumpSound);
	}
	self->ay = self->charRec->gravity;
	self->vy = self->charRec->jumpVy;
	self->maxVx = self->charRec->airMaxVx;
}
