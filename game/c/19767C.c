/* Wolverine_LeapSlashEnd - after a leaping slash (Wolverine_GroundSpecial, Wolverine_StateLunge):
 * unlatch A; XPlayer_AttackRecover follows when the animation ends (at once when it has).
 *
 * self: the player.
 * The delay is $FF while the animation plays (its end clears it), else 0. */
#include "xmen2.h"

void Wolverine_LeapSlashEnd(register Wolverine *self)
{
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskA;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskA;
	self->state = (StateFn)XPlayer_AttackRecover;
	self->timer = (unsigned char)(self->anim ? 0xFF : 0);
}
