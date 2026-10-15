/* Tusk_StateIntro - TUSK's first state: lock the arena (Tusk_LockArena), stand still with the
 * idle animation, stand on the type-$13 floor segment nearest to 32 px above it, and wait
 * 300 frames (Tusk_StateIntroWait).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_StateIntro(Tusk *self)
{
	Seg *seg;

	Tusk_LockArena(self);
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_TuskAnim));
	{
		short x = self->x >> 8;
		short y = (self->y >> 8) - 32;

		seg = Seg_FindNearestOfType(x, y, 0x13);
	}
	if (seg)
		Obj_StandOnSeg(self, seg);
	self->countdown = 300;
	self->state = Tusk_StateIntroWait;
	return 0;
}
