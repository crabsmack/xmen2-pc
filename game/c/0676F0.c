/* PortraitActor_StateIdle - SCALINGACTOR state: the portrait stands, replaying its idle pose
 * whenever no animation plays (PlayerSelect_PlayIdleAnim). When its slot asks for another
 * character: a player who had none takes it now and its portrait flies in
 * (PortraitActor_StateFirstChar); a shown portrait first flies out (PortraitActor_StartFlyOut).
 * Otherwise the request is dropped.
 * Native, the select_idles option: Magneto's pick (PlayerSelect_PlayPickAnim) goes on here, the
 * flux bomb's explosion sound and the shot's follow-through (Tbl_MagnetoAnim6) as its wind-up ends.
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
#if defined(XMEN2_NATIVE)
	if (self->steps < 0 && !self->anim) {	/* select_idles: Magneto's pick (PlayerSelect_PlayPickAnim), the wind-up */
		if (self->steps == -1) {	/* done: where the globe would leave, the flux bomb's explosion sound ($7C,
						   the user's pick of his sounds) and the follow-through */
			Snd_Stop(0x7C);
			Snd_Play(0x7C);
			Obj_SetAnim(self, CODEPTR(Tbl_MagnetoAnim6));
			self->steps = -2;
		} else
			self->steps = 0;
	}
#endif
	if (!self->anim)
		PlayerSelect_PlayIdleAnim(self);
	SET_STATE(self, PortraitActor_StateIdle);
	return self->flags & 8;
}
