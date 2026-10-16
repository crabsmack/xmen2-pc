/* Player_CeilingEndInView - for a player hanging from a ceiling (Cam_TrackPlayer): is the end of
 * that ceiling it moves towards on screen, at least the camera's wallMargin inside it?
 *
 * player: a climbing player (XCLIMBER; climbSeg is the ceiling).
 * Facing right: the right end at most $140 - wallMargin from the screen's left; facing left: the
 * left end at least wallMargin from it. Returns 1 / 0. */
#include "xmen2.h"

char Player_CeilingEndInView(register void *player)
{
	register Seg *seg = ((XClimber *)player)->climbSeg;
	register short x = ((Actor *)player)->x >> 8;	/* never used */
	register short margin = g_Camera->wallMargin;
	register short left = seg->x1;
	register short right = seg->x2;

	if (left > right) {
		left = seg->x2;
		right = seg->x1;
	}
	if (!(((Actor *)player)->spr.flags & 0x10))
		return right - g_CameraX <= 0x140 - margin;
	return left - g_CameraX >= margin;
}
