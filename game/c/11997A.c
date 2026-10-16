/* Synchronizer_Free - SYNCHRONIZER slot +$22: give its g_SyncPool slot back.
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_Free(Synchronizer *self)
{
	self->inUse = 0;
}
