/* TrapDoor_StateSpring - TRAPDOOR state: the door springs back: its closed blocks (slot +$82,
 * TrapDoor_SetTiles), its segment 2 px up, and each player present, alive and standing on it
 * 2 px up with it (Obj_UpdateBoxes); open in 5 frames (TrapDoor_StateOpen).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateSpring(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileClosed);	/* +$82 TrapDoor_SetTiles */
	self->door->y1 -= 2;
	self->door->y2 -= 2;
	if (IS_ALIVE(&g_Player1) && g_Player1.seg == self->door) {
		g_Player1.y -= 0x200;
		Obj_UpdateBoxes(&g_Player1);
	}
	if (IS_ALIVE(&g_Player2) && g_Player2.seg == self->door) {
		g_Player2.y -= 0x200;
		Obj_UpdateBoxes(&g_Player2);
	}
#if defined(XMEN2_NATIVE)
	P34_Each([&](XPlayer *q) {	/* players 3-4 in play too (R11) */
		if (IS_ALIVE(q) && q->seg == self->door) {
			q->y -= 0x200;
			Obj_UpdateBoxes(q);
		}
	});
#endif
	SET_STATE_WAIT(self, TrapDoor_StateOpen, 5);
	return self->flags & 8;
}
