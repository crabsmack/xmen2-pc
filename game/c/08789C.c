/* BroodBoss_ActScream - BROODBOSS AI-script action: scream (Tbl_BroodBossAnim9, sound $AE);
 * BroodBoss_StateScream when the animation ends.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActScream(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim9));
	Snd_Stop(0xAE);
	Snd_Play(0xAE);
	SET_STATE_WAIT(self, BroodBoss_StateScream, 0xFF);
	return self->flags & 8;
}
