/* Nightcrawler_StateTeleport - NIGHTCRAWLER travelling unseen: reappear (Nightcrawler_TeleportAppear,
 * now, stopped) when pushed by a segment, $B0 px or more from safeY, or out of teleportTime.
 *
 * self: the player.
 * Hit cooldown held at 60; the camera keeps both players in view; teleportTime -2 a frame (-4 in
 * two-player mode). Pushed: pushSeg is not $FF (XPlayer_Move resets it). */
#include "xmen2.h"

void Nightcrawler_StateTeleport(register Nightcrawler *self)
{
	register short dy = self->safeY - (self->y >> 8);

	self->hitCooldown = 60;
	Cam_ClampLookToPlayers(g_Camera);
	self->teleportTime -= g_TwoPlayerStep ? 4 : 2;
	if (self->pushSeg != 0xFF || ABS(dy) > 0xAF || self->teleportTime <= 0) {
		STOP_MOVING(self);
		GOTO_STATE(self, Nightcrawler_TeleportAppear);
	}
}
