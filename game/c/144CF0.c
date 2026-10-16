/* ToxicCan_Despawn - TOXICCAN Despawn (class slot +$36; the parent's isn't called): leave the
 * object table and hand the spawn record back, as a TOXICCANDAMAGED when it was damaged.
 *
 * self: the can.  rec: its spawn record.
 * flags = 0; a damaged can (frame 1) comes back damaged; the record: state 5, its position now
 * (pixels), no object ($FF). Its timer is freed (Timer_Free). */
#include "xmen2.h"

void ToxicCan_Despawn(register ToxicCan *self, register SpawnRec *rec)
{
	self->flags = 0;
	if (self->spr.frame == 1)
		rec->cls = CLASS(Tbl_ToxicCanDamaged_vtable);
	rec->flags = 5;
	rec->x = self->x >> 8;
	rec->y = self->y >> 8;
	rec->object = 0xFF;
	Timer_Free(self->timerObj);
	self->timerObj = 0;
}
