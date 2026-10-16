/* XClone_StateChildHome - XCLONE state: move the balls form 3/32 of the way to its target each
 * frame until it is within $200 (|dx| + |dy|, 8 fraction bits), then go on with the script on the
 * next update.
 *
 * self: the clone (state set by XClone_StateChildAim; never runs in the corpus).
 * Returns flags & 8 while homing; nothing in particular when the script goes on.
 * The script step is written out with a return in each arm (AI_NEXT_STEP's code): the statement
 * after it can never run (clearing the aiming flag $400), but MPW C emits it - the 6 bytes before
 * XClone_StateChildHome_Step, the disassembly's name for the homing code below. */
#include "xmen2.h"

short XClone_StateChildHome(XClone *self)
{
	BioBalls *balls = self->child;
	long dx, dy;

	dx = (balls->px << 8) - balls->x;
	dy = (balls->py << 8) - balls->y;
	if (ABS(dx) + ABS(dy) < 0x200) {
		if (!self->ai.table) {
			self->state = self->defaultState;
			self->timer = 0;
			RETURN_NO_VALUE;
		} else {
			self->ai.index++;
			if (self->ai.index >= AI_SCRIPT(self->ai)->count)
				self->ai.index = 0;
			self->state = FN_AT(StateFn, AI_SCRIPT(self->ai)->step + self->ai.index);
			self->timer = 0;
			RETURN_NO_VALUE;
		}
		balls->ballFlags &= ~0x400;		/* never reached */
	}
	/* XClone_StateChildHome_Step */
	balls->vx = (dx >> 3) - (dx >> 5);
	balls->vy = (dy >> 3) - (dy >> 5);
	balls->x += balls->vx;
	balls->y += balls->vy;
	SET_STATE(self, XClone_StateChildHome);
	return self->flags & 8;
}
