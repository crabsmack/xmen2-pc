/* Magneto_GroundSpecialDown - MAGNETO Down+A on the ground (class slot +$E2): a crouched flux
 * bomb straight ahead.
 *
 * self: the player.
 * Crouching (px, +$10, the crouch flag: 1); aimDir 7 facing right, 2 facing left; then the bomb
 * wind-up (Magneto_StartBomb) now. */
#include "xmen2.h"

void Magneto_GroundSpecialDown(Magneto *self)
{
	self->px = 1;
	if (!(self->spr.flags & 0x10))
		self->aimDir = 7;
	else
		self->aimDir = 2;
	GOTO_STATE(self, Magneto_StartBomb);
}
