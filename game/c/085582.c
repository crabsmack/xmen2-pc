/* Teleporter_StateWait - TELEPORTER state: wait for a player to step on the pad. While any live
 * player is climbing (Player_ClimbState) it only waits; otherwise a live player whose body box
 * touches the pad's 20x20 box around its position and who is on the ground (flags bit 12 clear)
 * starts the teleport: flags6 |= $1000, Teleporter_StateTeleport next frame.
 *
 * self: the pad.
 * Returns flags & 8; nothing while nobody is on it (state functions' results are not read
 * there). */
#include "xmen2.h"

short Teleporter_StateWait(Teleporter *self)
{
	short left = (self->x >> 8) - 10;
	short top = (self->y >> 8) - 10;
	short right = left + 0x14;
	short bottom = top + 0x14;
	long i;
	XPlayer *p;

	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p)) && (char)Player_ClimbState(p)) {
			SET_STATE(self, Teleporter_StateWait);
			return self->flags & 8;
		}
	}
	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p))
		    && left <= p->boxRight && right >= p->boxLeft && top <= p->boxBottom
		    && bottom >= p->boxTop && !(p->flags & 0x1000)) {
			self->flags6 |= 0x1000;
			SET_STATE(self, Teleporter_StateTeleport);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
