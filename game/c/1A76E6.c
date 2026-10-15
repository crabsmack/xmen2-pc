/* Nightcrawler_TeleportVanish - NIGHTCRAWLER the teleport starts moving: no frame shown, 60 frames
 * of hit cooldown, the speed for teleportDir (Nightcrawler_SetTeleportVelocity), then
 * Nightcrawler_StateTeleport from the next update.
 *
 * self: the player.
 * The camera keeps both players in view (Cam_ClampLookToPlayers).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_TeleportVanish(register Nightcrawler *self)
{
	Cam_ClampLookToPlayers(g_Camera);
	self->hitCooldown = 60;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	Nightcrawler_SetTeleportVelocity(self);
	SET_STATE(self, (StateFn)Nightcrawler_StateTeleport);
	return self->flags & 8;
}
