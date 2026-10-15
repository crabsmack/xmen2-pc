/* Magneto_AirAttackDown - MAGNETO Down+B in the air (class slot +$BE): shoot down, or
 * down-diagonally when Right or Left is held (and face that way).
 *
 * self: the player.
 * Aim (aimDir): 8 down-right with Right, 1 down-left with Left, else straight down, 9 facing
 * right, 0 facing left; then the air shot (Magneto_StartAirShot) now. */
#include "xmen2.h"

void Magneto_AirAttackDown(Magneto *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->aimDir = 8;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->aimDir = 1;
	} else if (!(self->spr.flags & 0x10))
		self->aimDir = 9;
	else
		self->aimDir = 0;
	GOTO_STATE(self, (StateFn)Magneto_StartAirShot);
}
