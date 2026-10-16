/* Blast_ClearFlags - BLAST class slot +$72: clear bits in its switch's flag word (switchFlags).
 *
 * self: the blast.  bits: the bits. */
#include "xmen2.h"

void Blast_ClearFlags(Blast *self, short bits)
{
	register s16 *flags = self->switchFlags;

	*flags &= ~bits;
}
