/* BioNpc_StunShake - one step of a BIONPC's stun (BioNpc_UpdateStunned): it shakes 4 px (right when
 * the stun frames left & 3 is 2, left when 0), the frames left go down by step, and its palette is
 * the saved one, + 2 (mod 4) while bit 1 of the frames left is set.
 *
 * self: the object.  step: frames (1, or 2 in two-player stepping). */
#include "xmen2.h"

void BioNpc_StunShake(register BioNpc *self, short step)
{
	register short left = self->stun & 0x3FFF;
	register short phase = left & 3;

	if (phase == 2)
		self->x += 0x400;
	else if (phase == 0)
		self->x -= 0x400;
	left -= step;
	self->stun = self->stun & 0xC000 | left;
	if ((self->stun & 0x3FFF) & 2) {
		register short pal = (self->stun >> 14) + 2;

		if (pal > 3)
			pal -= 4;
		self->spr.flags = self->spr.flags & ~3 | pal;
	} else
		self->spr.flags = self->spr.flags & ~3 | self->stun >> 14;
}
