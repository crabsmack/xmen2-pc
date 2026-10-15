/* BroodBoss_ActStomp - BROODBOSS AI-script action: stomp (Tbl_BroodBossAnim4) with rumble sound
 * $91; BroodBoss_StateStompLand when the animation ends.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActStomp(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim4));
	Snd_Stop(0x91);
	Snd_Play(0x91);
	SET_STATE_WAIT(self, BroodBoss_StateStompLand, 0xFF);
	return self->flags & 8;
}
