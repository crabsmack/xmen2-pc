/* ExodusBlast_StateFlyDown - EXODUSBLAST state while flying down: below y $301 (pixels, the
 * floor) it breaks the floor segment EXODUS sent it at, if any (Exodus_CheckFloorHit), and dies
 * from the next update (ExodusBlast_StateDie).
 *
 * self: the blast. Returns self->flags & 8 when it switches, no value while flying. */
#include "xmen2.h"

short ExodusBlast_StateFlyDown(register ExodusBlast *self)
{
	if (self->y > 0x30100) {
		Exodus_CheckFloorHit();
		SET_STATE(self, (StateFn)ExodusBlast_StateDie);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
