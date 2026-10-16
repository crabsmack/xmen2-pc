/* Blast_Nop66 - class slot +$66 of BLAST: does nothing.
 *
 * self: the object.
 * Every class has this body: whatever it did was compiled out (an if (0) block). The frame
 * it left (self loaded into a4, a3 and a 4-byte local reserved) is reproduced by the
 * unused locals below. */
#include "xmen2.h"

void Blast_Nop66(Blast *self)
{
	Blast *p;
	Blast *q;

	if (0) {
		p = self;
		q = p;
	}
	self;
}
