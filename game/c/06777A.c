/* PortraitActor_StartFlyOut - SCALINGACTOR state: start the portrait's flight out: its big sprite
 * (the character's frame, PlayerSelect_CalcPortraitPos) goes in 8 steps from where the portrait
 * stands (homeX + offX, $AE + offY) to the middle of its portrait frame (cursor + 12 / 26 plus
 * half the frame offsets), shrinking from full to half size. Then PortraitActor_StateFlyOut from
 * the next update.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StartFlyOut(register ScalingActor *self)
{
	register BigSprite *slot = self->slot;
	register long t;

	PlayerSelect_CalcPortraitPos(self);
	self->fromX = self->homeX + self->offX;
	self->fromY = self->offY + SELECT_HOME_Y;
	self->toX = slot->cursorX + 12 + (self->offX >> 1);
	self->toY = slot->cursorY + 26 + (self->offY >> 1);
	t = self->picW;
	slot->w = t << 16;
	slot->vw = -((t << 15) / 8);
	t = self->picH;
	slot->h = t << 16;
	slot->vh = -((t << 15) / 8);
	t = self->toX - self->fromX;
	slot->vx = (t << 16) / 8;
	t = self->toY - self->fromY;
	slot->vy = (t << 16) / 8;
	self->steps = 8;
	slot->x = self->fromX << 16;
	slot->y = self->fromY << 16;
	SET_STATE(self, PortraitActor_StateFlyOut);
	return self->flags & 8;
}
