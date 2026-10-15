/* BabyBio_ActAttachFlame - BABYBIO AI-script action: replace its flame by a new BIOFLAME that
 * follows it 15 px ahead and 28 px up, facing its way with its sprite priority; next step now.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActAttachFlame(BabyBio *self)
{
	short dx = 15;
	short dy = -28;
	BioFlame *flame;

	if (self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
	flame = (BioFlame *)(*CLASS(Tbl_BioFlame_vtable)->method[2])(0,		/* New */
		CLASS(Tbl_BioFlame_vtable), self->x, self->y, 0);
	if (flame) {
		self->flame = flame;
		if (self->spr.flags & 0x10)
			flame->spr.flags |= 0x10;
		flame->owner = (Actor *)self;
		flame->offX = dx;
		flame->offY = dy;
		if (self->spr.flags & 4)
			flame->spr.flags |= 4;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
