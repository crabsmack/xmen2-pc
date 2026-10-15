/* FarAcolyte_StateWaitPlayer - FARACOLYTE state: wait until the nearest player's x is inside
 * rangeLeft..rangeRight, then send the orb (FarAcolyte_StartOrb) after it for up to 120 frames
 * (FarAcolyte_StateOrbChase).
 *
 * self: the acolyte.
 * Returns self->flags & 8; nothing when there is no player. */
#include "xmen2.h"

short FarAcolyte_StateWaitPlayer(register FarAcolyte *self)
{
	short px;
	register XPlayer *p;

	px = self->x >> 8;
	p = Player_Nearest((Actor *)self);
	if (p) {
		px = p->x >> 8;
		if (px < self->rangeLeft || px > self->rangeRight) {
			SET_STATE(self, FarAcolyte_StateWaitPlayer);
			return self->flags & 8;
		} else {
			(*self->cls->method[26])(self);	/* +$6E FarAcolyte_StartOrb */
			self->countdown = 0x78;
			SET_STATE(self, FarAcolyte_StateOrbChase);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
