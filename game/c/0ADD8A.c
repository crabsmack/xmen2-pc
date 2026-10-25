/* TrapDoor_StateOpen - TRAPDOOR state: the door opens: its half-open blocks (slot +$82,
 * TrapDoor_SetTiles), each player standing on it is let go (Obj_Detach) and the segment stops
 * holding anyone (flags bit 6); BARBARIAN is told the segment is gone
 * (Barbarian_NotifySegmentGone); wide open in 8 frames (TrapDoor_StateOpenWide).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateOpen(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileOpen);	/* +$82 TrapDoor_SetTiles */
	if (g_Player1.seg == self->door) {
		Obj_Detach(&g_Player1);
		self->door->flags |= 0x40;
	}
	if (g_Player2.seg == self->door) {
		Obj_Detach(&g_Player2);
		self->door->flags |= 0x40;
	}
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		if (q->seg == self->door) {
			Obj_Detach(q);
			self->door->flags |= 0x40;
		}
	});
#endif
	Barbarian_NotifySegmentGone(self->door);
	SET_STATE_WAIT(self, TrapDoor_StateOpenWide, 8);
	return self->flags & 8;
}
