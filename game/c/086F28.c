/* BroodBoss_ActDropBomb2 - BROODBOSS AI-script action: the bomb-drop animation (Tbl_BroodBossAnim5)
 * with rumble sound $91; BroodBoss_StateDropBomb2 (the bomb further right) when it ends.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_ActDropBomb2(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim5));
	Snd_Stop(0x91);
	Snd_Play(0x91);
	SET_STATE_WAIT(self, BroodBoss_StateDropBomb2, 0xFF);
	return self->flags & 8;
}
