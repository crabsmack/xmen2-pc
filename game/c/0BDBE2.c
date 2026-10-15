/* Barbarian_StateTaunt - BARBARIAN state: taunt - play the next taunt animation (cycle
 * tauntIndex 0-3; all four are Tbl_BarbarianAnim4), wait for its end, then idle
 * (Barbarian_StateIdle). With taunts > 100 it scrolls the map instead
 * (Barbarian_StateScrollTiles, in 60 frames); nothing counts taunts up, so that never happens.
 *
 * self: the object.
 * Returns self->flags & 8.
 * The cases are the disassembly's fragments Barbarian_StateTaunt_Reset (default),
 * Barbarian_StateTaunt_Play (case 1) and Barbarian_StateTaunt_Wait (after the switch). */
#include "xmen2.h"

short Barbarian_StateTaunt(Barbarian *self)
{
	if (self->taunts > 100) {
		SET_STATE_WAIT(self, Barbarian_StateScrollTiles, 60);
		return self->flags & 8;
	}
	switch (++self->tauntIndex) {
	default:			/* Barbarian_StateTaunt_Reset */
		self->tauntIndex = 0;
	case 0:
		Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
		break;
	case 1:				/* Barbarian_StateTaunt_Play */
		Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
		break;
	case 2:
		Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
		break;
	case 3:
		Obj_SetAnim(self, CODEPTR(Tbl_BarbarianAnim4));
		break;
	}
	SET_STATE_WAIT(self, Barbarian_StateIdle, 0xFF);	/* Barbarian_StateTaunt_Wait */
	return self->flags & 8;
}
