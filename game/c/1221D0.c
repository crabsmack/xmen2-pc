/* BigBio_StateStartMorph - BIGBIO: hide the body, put the orbs on the shape and jump to the arena
 * side it faces (50 px from the left edge, or 350), then slide the orbs into place
 * (BigBio_StateMorphToShape).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateStartMorph(register BigBio *self)
{
	self->orbMode |= 8;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	BigBio_SetOrbShape(self, (BigBioShapePt *)CODEPTR(Tbl_BigBioOrbShape));
	if (self->facingRight)
		self->x = (self->arenaLeft + 350) << 8;
	else
		self->x = (self->arenaLeft + 50) << 8;
	SET_STATE(self, BigBio_StateMorphToShape);
	return self->flags & 8;
}
