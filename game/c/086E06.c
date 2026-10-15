/* BroodBoss_ActClaw - BROODBOSS AI-script action: roar (Tbl_BroodBossAnim) and swing the claw out
 * (speed -$200, mode 1: the swing scrolls the arena), next step at once.
 *
 * self: the boss. */
#include "xmen2.h"

void BroodBoss_ActClaw(BroodBoss *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BroodBossAnim));
	self->claw->vx = -0x200;
	self->claw->mode = 1;
	self->claw->motion = BroodClaw_Swing;
	AI_NEXT_STEP(self, self->ai, 0);
}
