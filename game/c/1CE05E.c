/* Magneto_AirAttackUp - MAGNETO Up+B in the air (class slot +$BA): shoot up, or up-diagonally
 * when Right or Left is held (and face that way).
 *
 * self: the player.
 * Aim (aimDir): 6 up-right with Right, 3 up-left with Left, else straight up, 5 facing right,
 * 4 facing left; then the air shot (Magneto_StartAirShot) now. */
#include "xmen2.h"

void Magneto_AirAttackUp(Magneto *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		self->spr.flags &= ~0x10;
		self->aimDir = 6;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		self->spr.flags |= 0x10;
		self->aimDir = 3;
	} else if (!(self->spr.flags & 0x10))
		self->aimDir = 5;
	else
		self->aimDir = 4;
	GOTO_STATE(self, (StateFn)Magneto_StartAirShot);
}
