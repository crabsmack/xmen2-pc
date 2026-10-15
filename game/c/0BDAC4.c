/* Barbarian_StateIdle - BARBARIAN state: idle - flags6 bit $1000 off, the idle animation
 * (Tbl_BarbarianAnim5), 200 frames before it taunts, then watch the players
 * (Barbarian_StateWatchP1).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateIdle(Barbarian *self)
{
	self->flags6 &= ~0x1000;
	Obj_ChangeAnim(self, CODEPTR(Tbl_BarbarianAnim5));
	self->countdown = 200;
	SET_STATE(self, Barbarian_StateWatchP1);
	return self->flags & 8;
}
