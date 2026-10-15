/* XPlayer_Crouch - XPLAYER class slot +$B2: crouch: stop, the crouching animation, and the
 * crouching state (XPlayer_StateCrouch), now.
 *
 * self: the player.
 * stateTimer = 60: the delay before crouching turns into looking down. The animation and the
 * state change sit in one do/while, as in XPlayer_StartFloatFall (one pop at the end). */
#include "xmen2.h"

void XPlayer_Crouch(XPlayer *self)
{
	STOP_MOVING(self);
	do {
		Obj_ChangeAnim(self, CHAR_PART(self->charRec, crouchAnim));
		self->stateTimer = 60;
		self->state = (StateFn)XPlayer_StateCrouch;
		XPlayer_StateCrouch(self);
	} while (0);
}
