/* FarAcolyte_StateOrbChase - FARACOLYTE state: move the orb towards the nearest player
 * (FarAcolyte_MoveOrb). When it gets there: the orb flickers, sound $D9, and the acolyte casts
 * in 20 frames (FarAcolyte_StateCast). When the countdown runs out first: the orb is dropped
 * and it waits for the player again (FarAcolyte_StateWaitPlayer) after restDelay frames, or
 * right now (restDelay -1).
 *
 * self: the acolyte.
 * Returns self->flags & 8 after a state change, nothing otherwise (or what
 * FarAcolyte_StateWaitPlayer returned). */
#include "xmen2.h"

short FarAcolyte_StateOrbChase(register FarAcolyte *self)
{
	if (self->countdown <= 0) {
		self->shotFlags &= ~1;
		if (self->restDelay == -1)
			GOTO_STATE(self, FarAcolyte_StateWaitPlayer);
		else {
			self->timer = self->restDelay;
			self->state = FarAcolyte_StateWaitPlayer;
			return self->flags & 8;
		}
	} else if ((char)(*self->cls->method[27])(self)) {	/* +$72 FarAcolyte_MoveOrb */
		self->shotFlags |= 2;
		Snd_Stop(0xD9);
		Snd_Play(0xD9);
		SET_STATE_WAIT(self, FarAcolyte_StateCast, 20);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
