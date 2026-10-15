/* Nightcrawler_AirAttackDown - NIGHTCRAWLER Down+B in the air (class slot +$BE): the dive kick,
 * once per jump, drifting forward when Left or Right is held.
 *
 * self: the player.
 * Dive used (playerFlags 2): back to the control state (slot +$A6) from the next update.
 * Else sound $84, vy 0, gravity $B4, maximum fall speed $860, countdown 6, playerFlags 2 (used)
 * and 1 (diving), tells the camera; with Left/Right held the drifting dive (Tbl_Anim_1A6068),
 * acceleration $96 ahead, maximum speed $730; else straight down (Tbl_Anim_1A6060), stopped in
 * x. Then Nightcrawler_StateDiveHold.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_AirAttackDown(register Nightcrawler *self)
{
	if (self->playerFlags & 2) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	Snd_Stop(0x84);
	Snd_Play(0x84);
	self->vy = 0;
	self->ay = 0xB4;
	self->maxVy = 0x860;
	self->countdown = 6;
	self->playerFlags |= 2;
	self->playerFlags |= 1;
	g_CamModeSwitched = 1;
	if (!self->pad ? g_Pad1Held & g_PadMaskLeftRight : g_Pad2Held & g_PadMaskLeftRight) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A6068));
		self->ax = self->spr.flags & 0x10 ? -0x96 : 0x96;
		self->maxVx = 0x730;
	} else {
		self->ax = 0;
		self->vx = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A6060));
	}
	SET_STATE(self, Nightcrawler_StateDiveHold);
	return self->flags & 8;
}
