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
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: unless another player is present and alive */
		XPlayer *o[3];

		if (!P34_OthersAlive(self, o))
			Scene_EndAfter(120);
		return;
	}
#endif
	if (!IS_ALIVE(partner))
		Scene_EndAfter(120);
}
