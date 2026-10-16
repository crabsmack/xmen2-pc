/* BrainBabyBio_OnDeath - BRAINBABYBIO OnDeath (class slot +$72): it turns into a SAVAGEMEDIUMBIO
 * (its New, slot +$0E, at its position) that faces the same way (sprite flag $10), flashes 20
 * frames (no hits) and has its home at ($18E, $140) (SavageMediumBio_SetHome); then it removes
 * itself (slot +$52).
 *
 * self: the object. */
#include "xmen2.h"

void BrainBabyBio_OnDeath(register BrainBabyBio *self)
{
	SavageMediumBio *bio;

	if ((bio = (SavageMediumBio *)(*CLASS(Tbl_SavageMediumBio_vtable)->method[2])(0,
	    CLASS(Tbl_SavageMediumBio_vtable), self->x, self->y, 0))) {	/* New */
		if (self->spr.flags & 0x10)
			bio->spr.flags |= 0x10;
		bio->hitCooldown = 20;
		SavageMediumBio_SetHome(bio, 0x18E, 0x140);
	}
	(*self->cls->method[19])(self);	/* +$52 Kill */
}
