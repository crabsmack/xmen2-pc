/* BigBio_ActRoarThenMorph - BIGBIO script action: face the way +$B6 says, roar (animation, sound
 * $F9), then BigBio_StateStartMorph once the animation has played.
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_ActRoarThenMorph(register BigBio *self)
{
	if (self->facingRight)
		self->spr.flags &= ~0x10;
	else
		self->spr.flags |= 0x10;
	Obj_SetAnim(self, CODEPTR(Tbl_BigBioAnim));
	Snd_Stop(0xF9);
	Snd_Play(0xF9);
	SET_STATE_WAIT(self, BigBio_StateStartMorph, 0xFF);
	return self->flags & 8;
}
