/* BabyBio_Deactivate - BABYBIO Deactivate (class slot +$46): kill its flame, then the parent's
 * (BIONPC) Deactivate.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_Deactivate(BabyBio *self)
{
	if (self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
	(*CLASS(Tbl_BabyBio_vtable)->parent->method[16])(self);	/* BIONPC Deactivate */
}
