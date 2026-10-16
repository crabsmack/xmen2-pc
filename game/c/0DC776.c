/* Barrier_Despawn - BARRIER Despawn (class slot +$36; the parent's isn't called): leave the
 * object table and hand the spawn record back, as a CLOSEDBARRIER when it had closed.
 *
 * self: the barrier.  rec: its spawn record.
 * flags = 0; a closed barrier's record gets the CLOSEDBARRIER class (it comes back shut); the
 * record: state 5, its position now (pixels), no object ($FF). Its timer is freed (Timer_Free). */
#include "xmen2.h"

void Barrier_Despawn(register Barrier *self, register SpawnRec *rec)
{
	self->flags = 0;
	if (self->closed)
		rec->cls = CLASS(Tbl_ClosedBarrier_vtable);
	rec->flags = 5;
	rec->x = self->x >> 8;
	rec->y = self->y >> 8;
	rec->object = 0xFF;
	Timer_Free(self->timerObj);
	self->timerObj = 0;
}
