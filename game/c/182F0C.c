/* Beast_DiveImpact - the dive (Beast_AirAttackDown) hits the ground or a target: Beast bounces up
 * off it and the screen shakes.
 *
 * self: the player.
 * On the ground he leaves it (Obj_Detach, playerFlags 8: as after a jump). vy -$700, the air
 * speed, gravity and fall speed of his record, no speed or acceleration x; the control state
 * (slot +$A6) once the bounce animation (Tbl_Anim_1829A6) ends. playerFlags 1 and 2 (diving,
 * dive used) are cleared, B and A unlatched; the dive sound $96 stops, $9A plays and the camera
 * shakes (Tbl_BeastQuakeSmall). From Beast_StateDive(Hold) on landing and Beast_Attack. */
#include "xmen2.h"

void Beast_DiveImpact(register Beast *self)
{
	if (!(self->flags & 0x1000)) {
		Obj_Detach(self);
		self->playerFlags |= 8;
	}
	self->maxVx = self->charRec->airMaxVx;
	self->vy = -0x700;
	self->ay = self->charRec->gravity;
	self->maxVy = self->charRec->maxFallVy;
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->vx = 0;
	self->ax = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829A6));
	self->timer = 0xFF;
	self->playerFlags &= ~2;
	self->playerFlags &= ~1;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
	Snd_Stop(0x96);
	Snd_Stop(0x9A);
	Snd_Play(0x9A);
	g_CamShake = CODEPTR(Tbl_BeastQuakeSmall);
}
