/* XClone_WalkStep - XCLONE: walk on in the facing direction (run animation, vx = walkSpeed)
 * unless it stands at that side's arena edge.
 *
 * self: the clone.
 * Returns 1 when walking, 0 at the edge (its speed is left as it was). */
#include "xmen2.h"

char XClone_WalkStep(XClone *self)
{
	short px;
	char walking;

	px = self->x >> 8;
	walking = 1;
	if (px == self->arenaRight && !(self->spr.flags & 0x10)
	    || px == self->arenaLeft && self->spr.flags & 0x10)
		walking = 0;
	else {
		Obj_ChangeAnim(self, CLONE_PART(self->charRec, runAnim));
		self->vx = self->spr.flags & 0x10 ? -self->walkSpeed : self->walkSpeed;
	}
	return walking;
}
