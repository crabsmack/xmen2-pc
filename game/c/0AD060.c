/* PerimeterDoor_StateWaitExit - PERIMETERDOOR state once the door is open: when a player is within
 * 16 px (x, and y from $30 px below the door's position) of the opening, end the scene in 10
 * frames (Scene_EndAfter) and stop (PerimeterDoor_StateDone).
 *
 * self: the door.
 * The players are tested whether alive or not.
 * Returns self->flags & 8 when the scene ends; nothing in particular while waiting. */
#include "xmen2.h"

short PerimeterDoor_StateWaitExit(PerimeterDoor *self)
{
	register short dx1 = ABS(self->x - g_Player1.x) >> 8;
	register short dy1 = ABS(self->y + 0x3000 - g_Player1.y) >> 8;
	register short dx2 = ABS(self->x - g_Player2.x) >> 8;
	register short dy2 = ABS(self->y + 0x3000 - g_Player2.y) >> 8;

	if (dx1 < 0x10 && dy1 < 0x10 || dx2 < 0x10 && dy2 < 0x10
#if defined(XMEN2_NATIVE)	/* phase 7: players 3-4 in play too, alive or not as these (R11) */
	    || P34_Any([&](XPlayer *q) { return (short)(ABS(self->x - q->x) >> 8) < 0x10
						&& (short)(ABS(self->y + 0x3000 - q->y) >> 8) < 0x10; })
#endif
	    ) {
		Scene_EndAfter(10);
		SET_STATE(self, (StateFn)PerimeterDoor_StateDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
