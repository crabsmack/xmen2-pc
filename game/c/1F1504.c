/* XPlayer_SaveSafePos - XPLAYER class slot +$A2: remember the floor segment the player stands on
 * and its position as the safe spot (XPlayer_PlaceAtSafePos puts it back there).
 *
 * self: the player. Runs every frame (XPlayer_Update); nothing changes while it is in the air. */
#include "xmen2.h"

void XPlayer_SaveSafePos(register XPlayer *self)
{
	Seg *seg;

	if (seg = self->seg) {
		self->safeSeg = seg;
		self->safeX = self->x >> 8;
		self->safeY = self->y >> 8;
	}
}
