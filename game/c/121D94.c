/* BigBio_SetOrbShape - remember an orb shape and put the orbs on it (BigBio_PlaceOrbs).
 *
 * self:  the boss.
 * shape: the shape (Tbl_BigBioOrbShape). Never runs in the corpus. */
#include "xmen2.h"

void BigBio_SetOrbShape(register BigBio *self, BigBioShapePt *shape)
{
	self->shape = shape;
	BigBio_PlaceOrbs(self, self->shape);
}
