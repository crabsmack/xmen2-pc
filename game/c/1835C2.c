/* Beast_StateStompEnd - the stomp has landed (Beast_StateStomp): above 7 health units the two
 * shockwaves set off (playerFlags $80000 left, $100000 right: Beast_UpdateShockwaves moves
 * them); then the recovery animation, and XPlayer_AttackRecover at its end.
 *
 * self: the player.
 * A is unlatched and the stomp's invulnerability (hitCooldown) ends. */
#include "xmen2.h"

void Beast_StateStompEnd(register Beast *self)
{
	if (self->health >= 0x701) {
		self->playerFlags |= 0x80000;
		self->playerFlags |= 0x100000;
	}
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
	Obj_SetAnim(self, CODEPTR(Tbl_EvilBeastAnim2));
	self->hitCooldown = 0;
	self->state = (StateFn)XPlayer_AttackRecover;
	self->timer = 0xFF;
}
