/* Tusk_AtkSpawnTuskette2 - TUSK attack step: send a TUSKETTE of kind 2 (leaves TUSK's ledge to
 * the left) from 16 px above it, and choose again after 60 frames (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0. The new TUSKETTE is kept in a variable that is never used. */
#include "xmen2.h"

short Tusk_AtkSpawnTuskette2(Tusk *self)
{
	Tuskette *t;

	t = Tuskette_Spawn(self->x, self->y - 0x1000, 2);
	self->countdown = 60;
	self->state = Tusk_StateChooseAttack;
	return 0;
}
