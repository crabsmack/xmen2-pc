/* BigBio_ActChargePlayer - BIGBIO script action: spin the orb ring once and charge at the centre of
 * the nearest player's body box (speed 3/64 of the distance per frame, braking after 18 frames:
 * BigBio_StateBrake). While the scene is ending (g_SceneExitTimer) it skips to the next step.
 *
 * self: the boss. Returns flags & 8 when it charges. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_ActChargePlayer(register BigBio *self)
{
	register XPlayer *p;
	long dx, dy;

	p = Player_Nearest((Actor *)self);
	BigBio_UpdateOrbRing(self);
	if (g_SceneExitTimer) {
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		STOP_MOVING(self);
		self->targetX = (p->boxRight + p->boxLeft) << 7;
		self->targetY = (p->boxTop + p->boxBottom) << 7;
		dx = self->targetX - self->x;
		dy = self->targetY - self->y;
		self->vx = (short)((dx >> 4) - (dx >> 6));
		self->vy = (short)((dy >> 4) - (dy >> 6));
		self->countdown = 18;
		SET_STATE(self, BigBio_StateBrake);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
