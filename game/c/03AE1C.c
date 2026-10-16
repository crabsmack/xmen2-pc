/* RollingDiscs_Nop66 - class slot +$66 of ROLLINGDISCS: does nothing.
 *
 * self: the object. Never runs in the corpus.
 * Every class has this body: whatever it did was compiled out (an if (0) block). The frame
 * it left (self loaded into a4, a3 and a 4-byte local reserved) is reproduced by the
 * unused locals below. */
#include "xmen2.h"

void RollingDiscs_Nop66(RollingDiscs *self)
{
	RollingDiscs *p;
	RollingDiscs *q;

	if (0) {
		p = self;
		q = p;
	}
	self;
}
