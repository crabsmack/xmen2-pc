/* Beast_AirSpecial - BEAST A in the air (class slot +$C6): his air special, once per jump, played
 * as an air attack (XPlayer_StateAirAttack).
 *
 * self: the player.
 * Used already (playerFlags 4, which the double jump also sets): the control state (slot +$A6)
 * from the next frame, returning self->flags & 8. Else normal gravity, playerFlags 4, the
 * animation Tbl_Anim_1829B2 and sound $BD; returns nothing. */
#include "xmen2.h"

short Beast_AirSpecial(register Beast *self)
{
	if (self->playerFlags & 4) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	self->ay = self->charRec->gravity;
	self->playerFlags |= 4;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829B2));
	Snd_Stop(0xBD);
	Snd_Play(0xBD);
	self->state = (StateFn)XPlayer_StateAirAttack;
	self->timer = 0;
	NO_RETURN_VALUE
}
