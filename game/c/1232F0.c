/* BigBio_StateDeathOrbs - BIGBIO dying: the body hidden, the orbs on the orb shape (tile frame 1),
 * for 120 frames, then they fall (BigBio_StateDying).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateDeathOrbs(register BigBio *self)
{
	self->orbMode |= 8;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	BigBio_SetOrbShape(self, (BigBioShapePt *)CODEPTR(Tbl_BigBioOrbShape));
	self->orbFrame = 1;
	self->countdown = 120;
	SET_STATE(self, BigBio_StateDying);
	return self->flags & 8;
}
