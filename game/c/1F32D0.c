/* XPlayer_SetWalkAnim - play the walking or the running animation, by speed.
 *
 * self: the player.
 * slow: animation below the character's run speed (the player then faces the way it moves).
 * fast: animation at or above it. */
#include "xmen2.h"

void XPlayer_SetWalkAnim(XPlayer *self, void *slow, void *fast)
{
	if (ABS(self->vx) >= self->charRec->runSpeed)
		Obj_ChangeAnim(self, fast);
	else {
		if (self->vx > 0)
			self->spr.flags &= ~0x10;
		else if (self->vx < 0)
			self->spr.flags |= 0x10;
		Obj_ChangeAnim(self, slow);
	}
}
