/* Hologram_WaitPlayersInArena - HOLOGRAM state: once both players are in the arena (an absent
 * player counts as in), lock the camera on it and close the gate (Hologram_LockArena), then
 * start the AI script (the intro: Hologram_StartAttackScript) in 60 frames.
 *
 * self: the hologram. */
#include "xmen2.h"

void Hologram_WaitPlayersInArena(register Hologram *self)
{
	if (Hologram_PlayerInArena(self, &g_Player1) && Hologram_PlayerInArena(self, &g_Player2)) {
		Hologram_LockArena(self);
		AI_FIRST_STEP(self, self->ai, 60);
	}
}
