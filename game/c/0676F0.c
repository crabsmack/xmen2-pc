/* PortraitActor_StateIdle - SCALINGACTOR state: the portrait stands, replaying its idle pose
 * whenever no animation plays (PlayerSelect_PlayIdleAnim). When its slot asks for another
 * character: a player who had none takes it now and its portrait flies in
 * (PortraitActor_StateFirstChar); a shown portrait first flies out (PortraitActor_StartFlyOut).
 * Otherwise the request is dropped.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateIdle(register ScalingActor *self)
{
	if (self->slot->changing) {
		if (self->slot->charIndex == -1 && self->slot->savedChar != -1) {
			self->slot->charIndex = self->slot->savedChar;
			SET_STATE(self, PortraitActor_StateFirstChar);
			return self->flags & 8;
		}
		if (self->spr.def) {
			SET_STATE(self, PortraitActor_StartFlyOut);
			return self->flags & 8;
		}
	}
	self->slot->changing = 0;
	if (!self->anim)
		PlayerSelect_PlayIdleAnim(self);
	SET_STATE(self, PortraitActor_StateIdle);
	return self->flags & 8;
}
