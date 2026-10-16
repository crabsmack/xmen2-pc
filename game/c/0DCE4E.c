/* Blast_SetFlags - BLAST class slot +$6E: set bits in its switch's flag word (switchFlags).
 *
 * self: the blast.  bits: the bits. */
#include "xmen2.h"

void Blast_SetFlags(Blast *self, short bits)
{
	register s16 *flags = self->switchFlags;

	*flags |= bits;
}
