/* XPlayer_StateRejoin - after falling off the screen: come back at the partner's safe spot when
 * this player can hold on there (XPlayer_CanRejoinAt), else at its own, and land there
 * (PlaceAtSafePos, slot +$9E), now.
 *
 * self: the player.
 * Clears flags6 $80. Taking the partner's spot, a climber also takes the partner's climb state
 * there (climbFlags bits 4-5; none when the partner is no climber). Tells the camera. */
#include "xmen2.h"

void XPlayer_StateRejoin(XPlayer *self)
{
	XPlayer *partner;
	unsigned short held;

	partner = self == &g_Player1 ? &g_Player2 : &g_Player1;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: at the first other player in play */
		XPlayer *o[3];

		if (P34_OthersAlive(self, o))
			partner = o[0];
	}
#endif
	self->flags6 &= ~0x80;
	if (XPlayer_CanRejoinAt(self, partner)) {
		self->safeX = partner->safeX;
		self->safeY = partner->safeY;
		self->safeSeg = partner->safeSeg;
		self->x = self->safeX << 8;
		self->y = self->safeY << 8;
		if (Player_IsClimber(self)) {
			if (Player_IsClimber(partner)) {
				held = (((XClimber *)partner)->climbFlags & 0x30) >> 4;
				((XClimber *)self)->climbFlags = ((XClimber *)self)->climbFlags & ~0x30 | held << 4;
			} else
				((XClimber *)self)->climbFlags = ((XClimber *)self)->climbFlags & ~0x30 | 0;
		}
	} else {
		self->x = self->safeX << 8;
		self->y = self->safeY << 8;
	}
	g_CamModeSwitched = 1;
	GOTO_STATE(self, (StateFn)self->cls->method[38]);	/* PlaceAtSafePos */
}
