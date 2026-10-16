/* BrainChild_OnDeath - BRAINCHILD OnDeath (class slot +$72): it turns into a BRAINBABYBIO (its
 * New, slot +$0E, at its position) that faces the same way (sprite flag $10), flashes 20 frames
 * (no hits) and has its home at ($8A, $140) (BrainBabyBio_SetHome); sound $F2 restarted; then it
 * removes itself (slot +$52).
 *
 * self: the object. */
#include "xmen2.h"

void BrainChild_OnDeath(register BrainChild *self)
{
	BrainBabyBio *baby;

	if ((baby = (BrainBabyBio *)(*CLASS(Tbl_BrainBabyBio_vtable)->method[2])(0,
	    CLASS(Tbl_BrainBabyBio_vtable), self->x, self->y, 0))) {	/* New */
		if (self->spr.flags & 0x10)
			baby->spr.flags |= 0x10;
		baby->hitCooldown = 20;
		BrainBabyBio_SetHome(baby, 0x8A, 0x140);
	}
	Snd_Stop(0xF2);
	Snd_Play(0xF2);
	(*self->cls->method[19])(self);	/* +$52 Kill */
}
