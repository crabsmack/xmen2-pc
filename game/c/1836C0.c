/* Beast_GroundSpecial - BEAST A on the ground (class slot +$DE): the stomp: his attack sound,
 * stop, the wind-up animation (Tbl_Anim_1829D0); Beast_StateStomp at its end.
 *
 * self: the player.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_GroundSpecial(register Beast *self)
{
	Snd_Stop(self->charRec->attackSound);
	Snd_Play(self->charRec->attackSound);
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829D0));
	SET_STATE_WAIT(self, (StateFn)Beast_StateStomp, 0xFF);
	return self->flags & 8;
}
