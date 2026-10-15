/* Exodus_StateDescend - EXODUS state: float down (vy $100, gravity $14; only y moves) to y $2A4
 * with its idle animation (Tbl_ExodusAnim), then wait to get there (Exodus_StateWaitLanded).
 *
 * self: the boss. Returns self->flags & 8.
 * Also where it starts again after rising (Exodus_StateRiseEnd) or leaving the arena
 * (Exodus_Update). */
#include "xmen2.h"

short Exodus_StateDescend(register Exodus *self)
{
	self->moveFlags = 0;
	self->moveFlags |= 1;
	self->vy = 0x100;
	self->ay = 0x14;
	self->targetY = 0x2A4;
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim));
	SET_STATE(self, Exodus_StateWaitLanded);
	return self->flags & 8;
}
