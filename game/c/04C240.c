/* Apocalypse_StateDefeated - APOCALYPSE state, out of health: no longer hittable (flags6 bit 1,
 * out of the enemy body hit list), stop, float to the centre (256, 96; damped move, shift 5)
 * with its defeat animation (Tbl_ApocalypseAnim); then Apocalypse_StateDefeatedWait.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateDefeated(register Apocalypse *self)
{
	self->flags6 |= 2;
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	STOP_MOVING(self);
	Apocalypse_MoveTo(self, 0x10000, 0x6000, 5);
	Obj_SetAnim(self, CODEPTR(Tbl_ApocalypseAnim));
	SET_STATE(self, Apocalypse_StateDefeatedWait);
	return self->flags & 8;
}
