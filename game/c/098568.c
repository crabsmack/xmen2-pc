/* EvilMagneto_ActBlast - EVILMAGNETO AI-script action: wind up a magnetic globe aimed the way it
 * faces (direction 7 right, 2 left when h-flipped) with Magneto's shot animation for it
 * (Magneto_ShotAnim); EvilMagneto_StateBlast fires at its end.
 *
 * self: the clone (never runs in the corpus).
 * Without an animation for the direction it goes on with the script on the next update instead.
 * Returns self->flags & 8 when the wind-up starts; nothing in particular otherwise. */
#include "xmen2.h"

short EvilMagneto_ActBlast(EvilMagneto *self)
{
	void *anim;

	if (!(self->spr.flags & 0x10))
		self->shotDir = 7;
	else
		self->shotDir = 2;
	if (anim = Magneto_ShotAnim(self->shotDir)) {
		Obj_SetAnim(self, anim);
		SET_STATE_WAIT(self, (StateFn)EvilMagneto_StateBlast, 0xFF);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
