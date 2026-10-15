/* Magneto_AirAttack - MAGNETO B in the air (class slot +$C2): shoot ahead, or diagonally up with
 * Up held, diagonally down with Down held.
 *
 * self: the player.
 * Aim (aimDir), facing right / left: Up 6 / 3, Down 8 / 1, else 7 / 2; then the air shot
 * (Magneto_StartAirShot) now. */
#include "xmen2.h"

void Magneto_AirAttack(Magneto *self)
{
	if (!self->pad ? g_Pad1Held & g_PadMaskUp : g_Pad2Held & g_PadMaskUp) {
		if (!(self->spr.flags & 0x10))
			self->aimDir = 6;
		else
			self->aimDir = 3;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskDown : g_Pad2Held & g_PadMaskDown) {
		if (!(self->spr.flags & 0x10))
			self->aimDir = 8;
		else
			self->aimDir = 1;
	} else if (!(self->spr.flags & 0x10))
		self->aimDir = 7;
	else
		self->aimDir = 2;
	GOTO_STATE(self, (StateFn)Magneto_StartAirShot);
}
