/* BioFlame_Nop66 - class slot +$66 of BIOFLAME: does nothing.
 *
 * self: the object.
 * Whatever it did was compiled out (an if (0) block). The frame it left (self loaded into a4,
 * a3 and a 4-byte local reserved) is reproduced by the unused locals below (as BabyBio_Nop66).
 * Never runs in the corpus. */
#include "xmen2.h"

void BioFlame_Nop66(BioFlame *self)
{
	BioFlame *p;
	BioFlame *q;

	if (0) {
		p = self;
		q = p;
	}
	self;
}
