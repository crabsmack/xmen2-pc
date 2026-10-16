/* TrapDoor_StateSag - TRAPDOOR state: the door sags: its sagging blocks (slot +$82,
 * TrapDoor_SetTiles), its segment 2 px down, and each player present, alive and standing on it
 * 2 px down with it (Obj_UpdateBoxes); spring back in 25 frames (TrapDoor_StateSpring).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short TrapDoor_StateSag(TrapDoor *self)
{
	(*self->cls->method[31])(self, self->tileSag);	/* +$82 TrapDoor_SetTiles */
	self->door->y1 += 2;
	self->door->y2 += 2;
	if (IS_ALIVE(&g_Player1) && g_Player1.seg == self->door) {
		g_Player1.y += 0x200;
		Obj_UpdateBoxes(&g_Player1);
	}
	if (IS_ALIVE(&g_Player2) && g_Player2.seg == self->door) {
		g_Player2.y += 0x200;
		Obj_UpdateBoxes(&g_Player2);
	}
	SET_STATE_WAIT(self, TrapDoor_StateSpring, 0x19);
	return self->flags & 8;
}
