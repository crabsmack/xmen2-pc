/* Actor_Despawn - ACTOR Despawn (class slot +$36): put a live object back into its spawn record.
 *
 * self: the object.  rec: its spawn record.
 * The object becomes unused (flags 0) and its timer is freed; the record waits again (flags 5:
 * waiting, bit 2) at the object's current position (pixels), with no object ($FF). */
#include "xmen2.h"

void Actor_Despawn(Actor *self, SpawnRec *rec)
{
	self->flags = 0;
	rec->flags = 5;
	rec->x = self->x >> 8;
	rec->y = self->y >> 8;
	rec->object = 0xFF;
	Timer_Free(self->timerObj);
	self->timerObj = 0;
}
