/* Tuskette_StateExitDrop - TUSKETTE leaving TUSK's arena, off the platform: sprite priority bit
 * on, behind in the draw list (DrawList_SetPriority with its layer and flags6 bit 3), drop
 * (vy $200, gravity 10) and fall (Tuskette_StateExitFall).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateExitDrop(Tuskette *self)
{
	self->spr.flags |= 4;
	DrawList_SetPriority(self->drawNode, 1, self->layer, self->flags6 & 8);
	self->vy = 0x200;
	self->ay = 10;
	SET_STATE(self, Tuskette_StateExitFall);
	return self->flags & 8;
}
