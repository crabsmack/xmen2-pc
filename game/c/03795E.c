/* MagnetoBoss_StateShootRecover - MAGNETOBOSS after a shot: the follow-through animation for
 * the shot's direction (Magneto_AirShotEndAnim), then, when it ends, the state to resume
 * (resumeState; none: stay).
 *
 * self: the boss. */
#include "xmen2.h"

void MagnetoBoss_StateShootRecover(register MagnetoBoss *self)
{
	register void *anim;

	anim = Magneto_AirShotEndAnim(self->shotDir);
	Obj_ChangeAnim(self, anim);
	if (self->resumeState) {
		self->state = self->resumeState;
		self->timer = 0xFF;
	}
}
