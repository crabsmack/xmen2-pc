/* Nightcrawler_TeleportDone - NIGHTCRAWLER the teleport is over: no hit cooldown, no longer
 * teleporting (px 0, playerFlags $40 and $200000 off; $800000 too once on the ground, else it
 * stays until he lands), the camera's flag $10 off, then the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void Nightcrawler_TeleportDone(register Nightcrawler *self)
{
	self->hitCooldown = 0;
	self->playerFlags &= ~0x40;
	self->px = 0;
	if (!(self->flags & 0x1000))
		self->playerFlags &= ~0x800000;
	self->playerFlags &= ~0x200000;
	g_Camera->camFlags &= ~0x10;
	GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
}
