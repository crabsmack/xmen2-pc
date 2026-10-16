/* SyncActor_IsTriggered - SYNCHRONIZEDACTOR slot +$7A: its group's SYNCHRONIZER is running: bit
 * $100 of its switch-board entry (Synchronizer_SetFlag).
 *
 * self: the object.  Returns $100 or 0. */
#include "xmen2.h"

short SyncActor_IsTriggered(Synchronizedactor *self)
{
	return g_BBoard->entry[self->unk_8A[1]] & 0x100;
}
