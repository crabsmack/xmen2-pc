/* SavageBio_StateChargeToPlayer - SAVAGEBIGBIO state: charge until within $82 px of the
 * nearest player, then stop and go on with the script. A player behind it makes it brake
 * (acceleration $18 against the motion) and, once slow, turn around.
 *
 * self: the boss (after SavageBio_ActChargePlayer; never runs in the corpus).
 * Returns self->flags & 8 while charging; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateChargeToPlayer(SavageBigBio *self)
{
	XPlayer *p;
	short px;
	short x = self->x >> 8;

	p = Player_Nearest((Actor *)self);
	px = p->x >> 8;
	if (ABS(x - px) < 0x82) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		if (!((self->spr.flags & 0x10) ? p->x < self->x : p->x > self->x)) {
			/* the player is behind: brake, and once slow turn to it */
			if (ABS(self->vx) < 8) {
				self->ax = 0;
				self->vx = 0;
				if (p->x < self->x)
					self->spr.flags |= 0x10;
				else
					self->spr.flags &= ~0x10;
			} else {
				self->ax = -((self->spr.flags & 0x10) ? -0x18 : 0x18);
			}
		} else {
			self->vx = (self->spr.flags & 0x10) ? -0x220 : 0x220;	/* ahead: full speed */
		}
		SET_STATE(self, SavageBio_StateChargeToPlayer);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
