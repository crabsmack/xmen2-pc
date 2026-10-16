/* TrapDoor_StateWaitPlayer - TRAPDOOR state: once a player stands on its segment, set flags6
 * bit $1000 and sag from the next update (TrapDoor_StateSag).
 *
 * self: the object. Returns self->flags & 8 when it switches, no value while waiting. */
#include "xmen2.h"

short TrapDoor_StateWaitPlayer(TrapDoor *self)
{
	if (g_Player1.seg == self->door || g_Player2.seg == self->door) {
		self->flags6 |= 0x1000;
		SET_STATE(self, TrapDoor_StateSag);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
