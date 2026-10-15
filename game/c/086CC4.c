/* BroodBoss_StateWaitArrive - BROODBOSS state: wait until the boss offset x is within 5 of $130
 * (the claw's swing pushes it there), then start the fight: g_BroodBossFlags bit 0, floor
 * segment type $17 hook CloneA5_SegHook17, and the AI script from step 0.
 *
 * self: the boss. Returns flags & 8 while it waits, nothing once the script starts. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

short BroodBoss_StateWaitArrive(BroodBoss *self)
{
	if (ABS((g_BroodBossOffsetX >> 8) - 0x130) < 5) {
		g_BroodBossFlags |= 1;
		g_SegTypeHooks[0x17] = (SegHookFn)CloneA5_SegHook17;
		AI_FIRST_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	SET_STATE(self, BroodBoss_StateWaitArrive);
	return self->flags & 8;
}
