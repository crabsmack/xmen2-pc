/* Wolverine_AirAttackDown - WOLVERINE Down+B in the air (class slot +$BE): the claw dive straight
 * down, once per jump.
 *
 * self: the player.
 * Dive used already (playerFlags 2): the control state (slot +$A6) from the next frame. Else the
 * dive animation (Tbl_WolverineAnim9), sound $88, vy 0, gravity $B4, fall speed up to $860,
 * countdown 6 (the dive can't end before), playerFlags 2; the camera is told
 * (g_CamModeSwitched); Wolverine_StateDiveHold follows.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_AirAttackDown(register Wolverine *self)
{
	if (self->playerFlags & 2) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim9));
	Snd_Stop(0x88);
	Snd_Play(0x88);
	self->vy = 0;
	self->ay = 0xB4;
	self->maxVy = 0x860;
	self->countdown = 6;
	self->playerFlags |= 2;
	g_CamModeSwitched = 1;
	SET_STATE(self, (StateFn)Wolverine_StateDiveHold);
	return self->flags & 8;
}
