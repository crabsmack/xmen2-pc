/* ICandy_Destroy - ICANDY Destroy (class slot +$1A): its frame must be uploaded again, its timer
 * stopped and freed, its draw-list node removed, its pool slot freed (ICandy_PoolFree).
 *
 * self: the object. */
#include "xmen2.h"

void ICandy_Destroy(register ICandy *self)
{
	self->sprFlags &= ~0x20;
	if (self->timer)
		Timer_Stop(self->timer);
	if (self->timer) {
		Timer_Free(self->timer);
		self->timer = 0;
	}
	if (self->drawNode > -1) {
		DrawList_Remove(self->drawNode);
		self->drawNode = -1;
	}
	ICandy_PoolFree(self);
}
