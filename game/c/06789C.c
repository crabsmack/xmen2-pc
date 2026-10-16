/* PortraitActor_StateFlyOut - SCALINGACTOR state: one step of the flight out: the big sprite moves
 * and shrinks one step, is scaled again (Cerebro_BigSpriteDecode) and shown. After the 8th step,
 * PortraitActor_StateSwapChar in 4 frames.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateFlyOut(register ScalingActor *self)
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
	slot->big = 1;
	if (--self->steps) {
		SET_STATE(self, PortraitActor_StateFlyOut);
		return self->flags & 8;
	} else {
		SET_STATE_WAIT(self, PortraitActor_StateSwapChar, 4);
		return self->flags & 8;
	}
}
