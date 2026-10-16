/* Tank_SetTrackLimits - a TANK's track: |half| pixels either side of its placement.
 *
 * self: the tank (its px/py, the placement position, Tank_Init).  half: the half-length (sign
 * ignored).  lo, hi: where the ends go (trackLeft, trackRight).
 * The right end is built from py, not px (a bug of the original, kept: Ninja_CalcPatrolBounds is
 * the same function with px). */
#include "xmen2.h"

void Tank_SetTrackLimits(Tank *self, register short half, s16 *lo, s16 *hi)
{
	*lo = self->px - ABS(half);
	*hi = self->py + ABS(half);
}
