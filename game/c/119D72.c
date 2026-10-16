/* SyncActor_Init - SYNCHRONIZEDACTOR Init (class slot +$12): the parent's (ACTOR) Init, its
 * switch-board entry (unk_8A[1]) from the low byte of placement param 0, and no timer of its own
 * (freed: its SYNCHRONIZER updates it).
 *
 * self: the object.  x, y: position (8 fraction bits).
 * pl:   its placement entry (must not be 0: read without a test). */
#include "xmen2.h"

void SyncActor_Init(register Synchronizedactor *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_SyncActor_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->unk_8A[1] = (u8)pl->param[0];	/* its switch-board entry */
	do {
		Timer_Free(self->timerObj);
		self->timerObj = 0;
	} while (0);
}
