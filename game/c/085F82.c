/* LittleLeg_Destroy - LITTLELEG Destroy (class slot +$16): stop and free its Timer and take it out of
 * the draw list (each only if it has one).
 *
 * self: the leg. */
#include "xmen2.h"

void LittleLeg_Destroy(LittleLeg *self)
{
	if (self->timer) {
		Timer_Stop(self->timer);
		Timer_Free(self->timer);
		self->timer = 0;
	}
	if (self->drawNode > -1) {
		DrawList_Remove(self->drawNode);
		self->drawNode = -1;
	}
}
