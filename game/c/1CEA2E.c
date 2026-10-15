/* Magneto_GroundSpecialUp - MAGNETO Up+A on the ground (class slot +$E6): a flux bomb up, or
 * diagonally up when Right or Left is held.
 *
 * self: the player.
 * Standing (px, +$10: 0); aim (aimDir): with Right or Left held 6 facing right, 3 facing left
 * (the way he faces, not the direction held, and he doesn't turn); else 5 / 4. Then the bomb
 * wind-up (Magneto_StartBomb) now. */
#include "xmen2.h"

void Magneto_GroundSpecialUp(Magneto *self)
{
	self->px = 0;
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		if (!(self->spr.flags & 0x10))
			self->aimDir = 6;
		else
			self->aimDir = 3;
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		if (!(self->spr.flags & 0x10))
			self->aimDir = 6;
		else
			self->aimDir = 3;
	} else if (!(self->spr.flags & 0x10))
		self->aimDir = 5;
	else
		self->aimDir = 4;
	GOTO_STATE(self, Magneto_StartBomb);
}
