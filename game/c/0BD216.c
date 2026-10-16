/* SavageBioBalls_ActBecomeBigBio - BIOBALLS script action (the end of the SAVAGEMEDIUMBIO's
 * burst): a SAVAGEBIGBIO at the balls' position (its New, slot +$0E) that faces the same way
 * (sprite flag $10) and flashes 20 frames (no hits); then the balls remove themselves (slot +$52).
 *
 * self: the balls (a step of their AI script). Returns nothing in particular. */
#include "xmen2.h"

short SavageBioBalls_ActBecomeBigBio(register BioBalls *self)
{
	register SavageBigBio *bio;

	bio = (SavageBigBio *)(*CLASS(Tbl_SavageBigBio_vtable)->method[2])(0, CLASS(Tbl_SavageBigBio_vtable),
		self->x, self->y, 0);	/* New */
	if (bio) {
		if (self->spr.flags & 0x10)
			bio->spr.flags |= 0x10;
		bio->hitCooldown = 20;
	}
	(*self->cls->method[19])(self);		/* +$52 Kill */
	NO_RETURN_VALUE
}
