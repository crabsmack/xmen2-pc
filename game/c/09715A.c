/* XClone_StateChildReturn - XCLONE state: move the balls form 1/16 of the way back to its start
 * point (px/py) each frame (twice in two-player step mode); within $200 (|dx| + |dy|, 8 fraction
 * bits) put it there and go on with the script now.
 *
 * self: the clone (state set by XClone_StateChildLeap; never runs in the corpus).
 * Returns nothing: the state stays until the script goes on. */
#include "xmen2.h"

void XClone_StateChildReturn(XClone *self)
{
	BioBalls *balls = self->child;
	long dx, dy;

	dx = (balls->px << 8) - balls->x;
	dy = (balls->py << 8) - balls->y;
	if (ABS(dx) + ABS(dy) < 0x200) {
		balls->x = balls->px << 8;
		balls->y = balls->py << 8;
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		dx >>= 4;
		dy >>= 4;
		balls->x += dx;
		balls->y += dy;
		if (g_TwoPlayerStep) {
			balls->x += dx;
			balls->y += dy;
		}
	}
}
