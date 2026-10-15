/* Beast_StateDive - diving after B was let go (Beast_StateDiveHold): the impact on landing
 * (Beast_DiveImpact); once countdown runs out, the dive ends in a normal fall.
 *
 * self: the player.
 * The dive's end: normal gravity, no acceleration x, the air and fall speeds of his record, B
 * unlatched, no animation, playerFlags 1 (diving) cleared and 2 (dive used) set; still in the
 * air, he moves up 15 px (back where Beast_AirAttackDown moved him from) and the camera is told
 * (g_CamModeSwitched). Then the control state (slot +$A6), now.
 * Returns self->flags & 8 while diving; nothing otherwise. */
#include "xmen2.h"

short Beast_StateDive(register Beast *self)
{
	if (!(self->flags & 0x1000)) {
		Beast_DiveImpact(self);
		RETURN_NO_VALUE;
	}
	if (self->countdown <= 0) {
		self->ay = self->charRec->gravity;
		self->ax = 0;
		self->maxVx = self->charRec->airMaxVx;
		self->maxVy = self->charRec->maxFallVy;
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->anim = 0;
		self->playerFlags &= ~1;
		self->playerFlags |= 2;
		if (self->flags & 0x1000) {
			self->y -= 0xF00;
			g_CamModeSwitched = 1;
		}
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		RETURN_NO_VALUE;
	}
	SET_STATE(self, (StateFn)Beast_StateDive);
	return self->flags & 8;
}
