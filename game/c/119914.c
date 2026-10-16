/* Synchronizer_SetFlag - SYNCHRONIZER slot +$16: set bit $100 (synchronizer running) of its
 * g_BBoard entry (SyncActor_IsTriggered tests it).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_SetFlag(Synchronizer *self)
{
	register s16 *entry = &g_BBoard->entry[self->board];

	*entry |= 0x100;
}
