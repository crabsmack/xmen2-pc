/* XPlayer_TetherClamp - two-player tether: set a coordinate to its clamped value, follow the
 * ground again, and record the side that blocked (XPlayer_TetherToPartner).
 *
 * self:  the player.
 * coord: its x or y.
 * value: the clamped value (8 fraction bits).
 * flag:  playerFlags bit to set ($4000 / $8000: blocked left / right).
 * On the ground the position is refitted to its segment (Seg_ObjPosUnclamped,
 * Seg_ApplyObjPos). */
#include "xmen2.h"

void XPlayer_TetherClamp(XPlayer *self, s32 *coord, long value, long flag)
{
	*coord = value;
	if (!(self->flags & 0x1000)) {
		Seg_ObjPosUnclamped(self->seg, self);
		Seg_ApplyObjPos(self);
	}
	self->playerFlags |= flag;
}
