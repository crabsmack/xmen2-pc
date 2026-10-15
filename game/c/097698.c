/* XClone_ActFacePlayer - XCLONE script action: turn to face the nearest player, go on with the
 * script in 10 frames.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActFacePlayer(XClone *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;	/* h-flip: face left */
	else
		self->spr.flags &= ~0x10;
	AI_NEXT_STEP(self, self->ai, 10);
}
