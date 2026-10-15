/* Tusk_AtkSpawnTusketteBoth - TUSK attack step: send a TUSKETTE of kind 2 (left) from 16 px
 * above it, and the kind-3 one (right) 30 frames later (Tusk_AtkSpawnTuskette3).
 *
 * self: TUSK.
 * Returns 0. The new TUSKETTE is kept in a variable that is never used. */
#include "xmen2.h"

short Tusk_AtkSpawnTusketteBoth(Tusk *self)
{
	Tuskette *t;

	t = Tuskette_Spawn(self->x, self->y - 0x1000, 2);
	self->state = Tusk_AtkSpawnTuskette3;
	self->timer = 30;
	return 0;
}
