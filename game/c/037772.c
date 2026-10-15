/* MagnetoBoss_StateFlyToFinal - MAGNETOBOSS: once at the final spot ($7C0, $100), hover inside
 * x $7A0-$864 (bossFlags bit 1, the limit-reached bits cleared) and wait for the camera
 * (MagnetoBoss_StateWaitCamera, also the state to resume after a shot).
 *
 * self: the boss.
 * "At the spot" is (x - $7C0) + (y - $100) < 16 in pixels, signed: any point up or left of it
 * passes. Returns self->flags & 8 when the state changes, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateFlyToFinal(register MagnetoBoss *self)
{
	if ((self->x >> 8) - 0x7C0 + ((self->y >> 8) - 0x100) < 16) {
		self->bossFlags &= ~0xC;
		self->bossFlags |= 2;
		self->minX = 0x7A0;
		self->maxX = 0x864;
		self->resumeState = MagnetoBoss_StateWaitCamera;
		SET_STATE(self, MagnetoBoss_StateWaitCamera);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
