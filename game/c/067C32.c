/* PortraitActor_StateFlyIn - SCALINGACTOR state: one step of the flight in: the big sprite moves
 * and grows one step and is scaled again (Cerebro_BigSpriteDecode). After the 8th step,
 * PortraitActor_StateLanded from the next update.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateFlyIn(register ScalingActor *self)
{
	register BigSprite *slot = self->slot;
	ScaleParams *p = slot->params;

	slot->x += slot->vx;
	slot->y += slot->vy;
	slot->w += slot->vw;
	slot->h += slot->vh;
	p->outW = slot->w >> 16;
	p->outH = slot->h >> 16;
	Cerebro_BigSpriteDecode(slot);
	if (--self->steps) {
		SET_STATE(self, PortraitActor_StateFlyIn);
		return self->flags & 8;
	} else {
		SET_STATE(self, PortraitActor_StateLanded);
		return self->flags & 8;
	}
}
