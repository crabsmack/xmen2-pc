/* Synchronizer_ClearFlag - SYNCHRONIZER slot +$1A: clear bit $100 of its g_BBoard entry.
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_ClearFlag(Synchronizer *self)
{
	register s16 *entry = &g_BBoard->entry[self->board];

	*entry &= ~0x100;
}
