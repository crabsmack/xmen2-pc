/* SwampFly_StateFly - SWAMPFLY state: fly along the path, one pair every stepTime frames
 * (doubled outside two-player stepping); at the path's end pick a new one from the next update
 * (SwampFly_StatePickPath).
 *
 * self: the fly.
 * It faces its target (h-flip) once the target is more than 20 px to one side. Each step its
 * position is home (px, py) + (dx << shiftX, dy << shiftY) of the pair reached (8 fraction
 * bits). The branch for a negative stepTime (skip steps) can't run: stepTime is unsigned, kept;
 * so is the dead "wait -1: go on at once" test. Returns self->flags & 8 (or the nested call's
 * result on that dead path). */
#include "xmen2.h"

/* Frames per step: stepTime, doubled outside two-player stepping. The dropped sign test (always
   false: stepTime is unsigned) is the ROM's dead tst.l before each use. */
#define STEP_FRAMES(o)	((void)((o)->stepTime < 0), (o)->stepTime << (g_TwoPlayerStep ? 0 : 1))

short SwampFly_StateFly(SwampFly *self)
{
	register FlyPathPt *pt;
	register long dx, dy, dist, step;

	step = self->pathStep;
	pt = &self->path->pt[step];
	dx = pt->dx;
	dy = pt->dy;
	dist = self->target->x - self->x;
	if (dist > 0x1400)
		self->spr.flags &= ~0x10;
	else if (dist < -0x1400)
		self->spr.flags |= 0x10;
	if (step < self->pathEnd) {
		self->x = (self->px << 8) + (dx << self->shiftX);
		self->y = (self->py << 8) + (dy << self->shiftY);
		if (self->stepTime < 0)
			self->pathStep -= self->stepTime >> g_TwoPlayerStep;
		else {
			self->pathStep++;
			if (STEP_FRAMES(self) == -1)
				GOTO_STATE(self, SwampFly_StateFly);
			else {
				SET_STATE_WAIT(self, SwampFly_StateFly, STEP_FRAMES(self));
				return self->flags & 8;
			}
		}
	} else {
		SET_STATE(self, SwampFly_StatePickPath);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
