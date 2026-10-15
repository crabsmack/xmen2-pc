/* Beast_StateRollEnd - the roll is over (Beast_StateRoll): the roll sound ($92) stops; on the
 * ground he uncurls at once (frame $83, Beast_SetFrameKeepCenterX) and goes back to the control
 * state (slot +$A6), now; in the air, once the animation ends, he takes the jump frame $D (box
 * centre kept: Beast_SetFrameKeepCenter) and recovers (Beast_StateRollRecover, countdown 6).
 *
 * self: the player.
 * Returns self->flags & 8 in the air; nothing on the ground. */
#include "xmen2.h"

short Beast_StateRollEnd(register Beast *self)
{
	Snd_Stop(0x92);
	if (self->flags & 0x1000) {
		if (!self->anim) {
			Beast_SetFrameKeepCenter(self, 0xD);
			self->countdown = 6;
			SET_STATE(self, (StateFn)Beast_StateRollRecover);
			return self->flags & 8;
		}
		SET_STATE(self, (StateFn)Beast_StateRollEnd);
		return self->flags & 8;
	}
	Beast_SetFrameKeepCenterX(self, 0x83);
	if (1) {	/* (not GOTO_STATE: no argument pop between the calls) */
		self->state = (StateFn)self->cls->method[40];	/* StateControl */
		(*(StateFn)self->cls->method[40])(self);
	}
	NO_RETURN_VALUE
}
