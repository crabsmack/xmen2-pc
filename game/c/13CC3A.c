/* Ninja_CalcPatrolBounds - a NINJA's patrol range: |half| pixels either side of its x.
 *
 * self: the ninja (its px, the placement x at Init).  half: the half-width (its sign ignored).
 * lo, hi: where the range's ends go (patrolLeft, patrolRight). */
#include "xmen2.h"

void Ninja_CalcPatrolBounds(Ninja *self, register short half, s16 *lo, s16 *hi)
{
	*lo = self->px - ABS(half);
	*hi = self->px + ABS(half);
}
