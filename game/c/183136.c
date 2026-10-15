/* Beast_AirAttackDown - BEAST Down+B in the air (class slot +$BE): dive straight down, once per
 * jump.
 *
 * self: the player.
 * Dive used already (playerFlags 2): the control state (slot +$A6) from the next frame. Else the
 * dive sound ($96), no speed, gravity $B4, fall speed up to $860, countdown 6 (the dive can't
 * end before), playerFlags 1 (diving) and 2, 15 px lower (the camera is told:
 * g_CamModeSwitched) and the dive animation (Tbl_Anim_18299C); Beast_StateDiveHold follows.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_AirAttackDown(register Beast *self)
{
	if (self->playerFlags & 2) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	Snd_Stop(0x96);
	Snd_Play(0x96);
	self->ax = 0;
	self->vx = 0;
	self->vy = 0;
	self->ay = 0xB4;
	self->maxVy = 0x860;
	self->countdown = 6;
	self->playerFlags |= 2;
	self->playerFlags |= 1;
	g_CamModeSwitched = 1;
	self->y += 0xF00;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_18299C));
	SET_STATE(self, (StateFn)Beast_StateDiveHold);
	return self->flags & 8;
}
