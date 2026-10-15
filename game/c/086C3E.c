/* BroodBoss_StateEnter - BROODBOSS state: the boss comes in. Its claw swings out (speed -$200,
 * mode 0: the swing pushes the boss offset x), sound $30, and the BROODBOSSDAMAGE hurt zones are
 * created at its position; then BroodBoss_StateWaitArrive.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateEnter(BroodBoss *self)
{
	self->claw->vx = -0x200;
	self->claw->mode = 0;
	self->claw->motion = BroodClaw_Swing;
	Snd_Stop(0x30);
	Snd_Play(0x30);
	(*CLASS(Tbl_BroodBossDamage_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_BroodBossDamage_vtable), self->px << 8, self->py << 8, 0);
	SET_STATE(self, BroodBoss_StateWaitArrive);
	return self->flags & 8;
}
