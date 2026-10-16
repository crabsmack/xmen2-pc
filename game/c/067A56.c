/* PortraitActor_StateFirstChar - SCALINGACTOR state: a player has just joined: its slot takes the
 * character (PlayerSelect_RevertCursor), becomes busy and hidden, the portrait takes the
 * character's sprite and frame (none: $FF), streamed into the slot's tiles, prepares its flight
 * picture (PlayerSelect_CalcPortraitPos) and flies in (PortraitActor_StartFlyIn, from the next
 * update).
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateFirstChar(register ScalingActor *self)
{
	PlayerSelect_RevertCursor(self->slot);
	self->slot->busy = 1;
	self->slot->hidden = 1;
	if ((self->spr.def = PlayerSelect_CharSpriteDef(self->slot)) != 0)
		self->frame = PlayerSelect_CharFrameNum(self->slot);
	else
		self->frame = 0xFF;
	self->spr.frame = self->frame;
	self->spr.tileBase = self->slot->tileBase;
	self->spr.tileCount = self->slot->tiles;
	self->spr.flags |= 0x80;
	PlayerSelect_CalcPortraitPos(self);
	SET_STATE(self, PortraitActor_StartFlyIn);
	return self->flags & 8;
}
