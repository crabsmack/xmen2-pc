/* Barbarian_StateSegmentEnd - BARBARIAN state: walked to the end of its segment - idle
 * animation (Tbl_BarbarianAnim5), flags6 bit $4000 on, no longer walking; then by leapMode:
 * 1 try the leap again (Barbarian_StateTryLeap2), else idle (Barbarian_StateIdle).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateSegmentEnd(Barbarian *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_BarbarianAnim5));
	self->flags6 |= 0x4000;
	self->walking = 0;
	switch (self->leapMode) {
	case 0:
		SET_STATE(self, Barbarian_StateIdle);
		return self->flags & 8;
	case 1:
		SET_STATE(self, Barbarian_StateTryLeap2);
		return self->flags & 8;
	default:
		SET_STATE(self, Barbarian_StateIdle);
		return self->flags & 8;
	}
}
