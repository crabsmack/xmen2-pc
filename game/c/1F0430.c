/* XPlayer_DeathDone - the death is over: remove the player, and end the scene in 120 frames
 * unless its partner is present and alive.
 *
 * self: the player. Its state stops for good (delay $FF). */
#include "xmen2.h"

void XPlayer_DeathDone(XPlayer *self)
{
	XPlayer *partner;

	Snd_Stop(0x52);
	(*self->cls->method[16])(self);		/* Deactivate */
	self->timer = 0xFF;
	partner = self == &g_Player1 ? &g_Player2 : &g_Player1;
	if (!IS_ALIVE(partner))
		Scene_EndAfter(120);
}
