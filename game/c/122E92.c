/* BigBio_ActRoarThenLaunch - BIGBIO script action: roar (animation, sound $F9), then leap
 * (BigBio_StateLaunch) once the animation has played.
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_ActRoarThenLaunch(register BigBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BigBioAnim));
	Snd_Stop(0xF9);
	Snd_Play(0xF9);
	SET_STATE_WAIT(self, BigBio_StateLaunch, 0xFF);
	return self->flags & 8;
}
