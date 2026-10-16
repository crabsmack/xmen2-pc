/* BioSentinel_StateHurt - BIOSENTINEL hit (its hurt state, BioSentinel_TakeDamage): no motion;
 * health over 2: the light hurt animation (Tbl_Anim_120688), under 3: the heavy one
 * (Tbl_Anim_120696), 40 frames of hit cooldown, BioSentinel_StateThink in 40 frames. No health
 * left: explode (class slot +$7E) with a long cooldown (10000) and stay in this state.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateHurt(register BioSentinel *self)
{
	STOP_MOVING(self);
	if (self->health > 2) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_120688));
		self->hitCooldown = 40;
		SET_STATE_WAIT(self, BioSentinel_StateThink, 40);
		return self->flags & 8;
	}
	if (self->health > 0) {
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_120696));
		self->hitCooldown = 40;
		SET_STATE_WAIT(self, BioSentinel_StateThink, 40);
		return self->flags & 8;
	}
	self->hitCooldown = 10000;
	(*self->cls->method[30])(self);		/* +$7E Explode */
	SET_STATE_WAIT(self, BioSentinel_StateHurt, 0xFF);
	return self->flags & 8;
}
