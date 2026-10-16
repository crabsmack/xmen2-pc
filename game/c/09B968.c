/* BroodClaw_StateIdle - BROODCLAW state: do nothing (run again next update): the first claw of a
 * pair (BroodClaw_Spawn) and the boss's claw at setup (BroodBoss_StateSetup).
 *
 * self: the claw.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short BroodClaw_StateIdle(BroodClaw *self)
{
	SET_STATE(self, BroodClaw_StateIdle);
	return self->flags & 8;
}
