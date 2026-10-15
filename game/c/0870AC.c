/* BroodBoss_StateShockOn - BROODBOSS state: the floor shock for 15 frames (g_BroodBossFlags bit 1:
 * BroodBoss_Update hits the players on the floor, CloneA5_AnimShockSprites shows it), sound $F5;
 * then BroodBoss_StateShockOff.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

short BroodBoss_StateShockOn(BroodBoss *self)
{
	g_BroodBossFlags |= 2;
	Snd_Stop(0xF5);
	Snd_Play(0xF5);
	SET_STATE_WAIT(self, (StateFn)BroodBoss_StateShockOff, 0xF);
	return self->flags & 8;
}
