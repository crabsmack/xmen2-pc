/* BigBio_StateWalkIn - BIGBIO entrance: walk until 80 px from the start, then stop, roar
 * (animation, sound $F2), leave the floor segment and start the script once the animation has
 * played (BigBio_StartScript).
 *
 * self: the boss (walking at +$D0 from BigBio_StateWaitPlayers). Returns flags & 8. */
#include "xmen2.h"

short BigBio_StateWalkIn(register BigBio *self)
{
	if (self->facingRight)
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	if (self->x - (self->px << 8) > 0x5000) {
		self->ax = 0;
		self->vx = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1206AC));
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		Obj_Detach(self);
		SET_STATE_WAIT(self, (StateFn)BigBio_StartScript, 0xFF);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateWalkIn);
	return self->flags & 8;
}
