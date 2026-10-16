/* Ninja_ActFacePlayer - NINJA script action: unless the nearest player is ahead of it, turn to
 * face it (h-flip = facing left; level with it: right); the idle animation Tbl_NinjaAnimIdle,
 * then the next script step from the next update.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActFacePlayer(register Ninja *self)
{
	register XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (!(self->spr.flags & 0x10 ? p->x < self->x : p->x > self->x)) {
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_NinjaAnimIdle));
	AI_NEXT_STEP(self, self->ai, 0);
}
