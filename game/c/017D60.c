/* FarAcolyte_StateBoltFly - FARACOLYTE state: move the bolt (FarAcolyte_MoveBolt). When it
 * reaches the orb: orb and bolt gone, a hurting explosion where the bolt is, and it waits for
 * the player again (FarAcolyte_StateWaitPlayer) after restDelay frames, or right now
 * (restDelay -1).
 *
 * self: the acolyte.
 * Returns self->flags & 8 after waiting is set, nothing otherwise (or what
 * FarAcolyte_StateWaitPlayer returned). */
#include "xmen2.h"

short FarAcolyte_StateBoltFly(register FarAcolyte *self)
{
	register FarAcolyteShot *bolt;
	long x, y;

	if ((char)(*self->cls->method[29])(self)) {	/* +$7A FarAcolyte_MoveBolt */
		bolt = &self->bolt;
		x = bolt->x;
		y = bolt->y;
		self->shotFlags &= ~7;
		Explosion_SpawnHurtingSetTimerWord(x, y, 1, 0, 4);
		if (self->restDelay == -1)
			GOTO_STATE(self, FarAcolyte_StateWaitPlayer);
		else {
			self->timer = self->restDelay;
			self->state = FarAcolyte_StateWaitPlayer;
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
