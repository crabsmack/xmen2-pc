/* PortraitActor_StartFlyIn - SCALINGACTOR state: start the portrait's flight in: its big sprite
 * goes in 8 steps from the middle of its portrait frame (cursor + 12 / 26 plus half the frame
 * offsets) to where the portrait stands (homeX + offX, $AE + offY), growing from half to full
 * size; its first step is scaled (Cerebro_BigSpriteDecode) and shown at once. Then
 * PortraitActor_StateFlyIn in 4 frames.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StartFlyIn(register ScalingActor *self)
{
	register BigSprite *slot = self->slot;
	ScaleParams *p = slot->params;
	register long t;

	self->toX = self->homeX + self->offX;
	self->toY = self->offY + SELECT_HOME_Y;
	self->fromX = slot->cursorX + 12 + (self->offX >> 1);
	self->fromY = slot->cursorY + 26 + (self->offY >> 1);
	t = self->picW;
	slot->w = t << 15;
	slot->vw = (t << 15) / 8;
	t = self->picH;
	slot->h = t << 15;
	slot->vh = (t << 15) / 8;
	t = self->toX - self->fromX;
	slot->vx = (t << 16) / 8;
	t = self->toY - self->fromY;
	slot->vy = (t << 16) / 8;
	self->steps = 8;
	slot->x = self->fromX << 16;
	slot->y = self->fromY << 16;
	p->outW = slot->w >> 16;
	p->outH = slot->h >> 16;
	Cerebro_BigSpriteDecode(slot);
	slot->big = 1;
	slot->hidden = 0;
	SET_STATE_WAIT(self, PortraitActor_StateFlyIn, 4);
	return self->flags & 8;
}
