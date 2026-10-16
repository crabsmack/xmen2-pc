/* GaussBullet_Nop0A - class slot +$0A of GAUSSBULLET: does nothing.
 *
 * self: the bullet.
 * Whatever it did was compiled out (an if (0) block). The frame it left (self loaded into a4,
 * a3 and a 4-byte local reserved) is reproduced by the unused locals below (as BabyBio_Nop66).
 * Never runs in the corpus. */
#include "xmen2.h"

void GaussBullet_Nop0A(GaussBullet *self)
{
	GaussBullet *p;
	GaussBullet *q;

	if (0) {
		p = self;
		q = p;
	}
	self;
}
