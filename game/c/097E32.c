/* EvilBeast_UpdateThrows - EVILBEAST class slot +$82 (each frame): move its stomp shockwaves on
 * (Beast_ShockwaveStep) until each ends.
 *
 * self: the clone.
 * waves bit 0: the left wave runs, bit 1: the right one (EvilBeast_StateThrow sets them); a wave
 * that ends clears its bit. Both run at the clone's current height, with sprite priority 1, hit
 * the players (not enemies) and are drawn with the scene's sprite g_EvilBeastThrowSprDef. */
#include "xmen2.h"

void EvilBeast_UpdateThrows(EvilBeast *self)
{
	s16 y;

	y = self->y >> 8;
	if (self->waves & 1) {
		if (Beast_ShockwaveStep(&self->waveLeftX, &y, &g_SpriteDefs[g_EvilBeastThrowSprDef],
			0, 1, 0))
			self->waves &= ~1;
	}
	if (self->waves & 2) {
		if (Beast_ShockwaveStep(&self->waveRightX, &y, &g_SpriteDefs[g_EvilBeastThrowSprDef],
			1, 1, 0))
			self->waves &= ~2;
	}
}
