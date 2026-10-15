/* Barbarian_StateStart - BARBARIAN first state (set by Init): stand on the floor segment found
 * from 20 px above its position, pose (Tbl_BarbarianAnim4), then idle (Barbarian_StateIdle).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateStart(Barbarian *self)
{
	Barbarian_StandOn(self, Seg_FindBelow(self->x >> 8, (self->y >> 8) - 20, 1000));
	Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
	SET_STATE(self, Barbarian_StateIdle);
	return self->flags & 8;
}
