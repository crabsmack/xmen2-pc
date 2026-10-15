/* XClone_StateChildAim - XCLONE state: set the balls form's speed to 3/32 of the distance to its
 * target (px/py), mark it aiming (ballFlags $400) and home in (XClone_StateChildHome).
 *
 * self: the clone (state set by XClone_ActChildAimPlayer; never runs in the corpus).
 * Returns flags & 8. */
#include "xmen2.h"

short XClone_StateChildAim(XClone *self)
{
	register BioBalls *balls = self->child;
	long dx, dy;

	dx = (balls->px << 8) - balls->x;
	dy = (balls->py << 8) - balls->y;
	balls->vx = (dx >> 3) - (dx >> 5);
	balls->vy = (dy >> 3) - (dy >> 5);
	balls->ballFlags |= 0x400;
	SET_STATE(self, XClone_StateChildHome);
	return self->flags & 8;
}
