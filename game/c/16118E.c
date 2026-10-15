/* Cam_EaseAxis - one camera coordinate eased toward keeping a target at a screen offset.
 *
 * ofs: the target's wanted screen offset; cam: the camera coordinate; step: the largest move
 * beyond the target's own; target, prev: the target's coordinate now and last frame.
 * The camera moves toward target - ofs, at most `step` more than the target moved, and never
 * against the target's motion. Returns the new camera coordinate (Cam_TrackPlayer: walls,
 * ledges, climbing, lifts). */
#include "xmen2.h"

long Cam_EaseAxis(short ofs, short cam, short step, short target, short prev)
{
	short want = target - ofs;
	short delta = want - cam;
	short moved = target - prev;
	short hi = moved + step;
	short lo = moved - step;

	if (lo > 0)
		lo = 0;
	if (hi < 0)
		hi = 0;
	return delta > hi ? cam + hi : delta < lo ? cam + lo : cam + delta;
}