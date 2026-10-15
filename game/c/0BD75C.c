/* Barbarian_TargetClose - is the target player within $32 px of BARBARIAN horizontally?
 *
 * self: the object (its target is set).
 * Returns 1 when |target x - x| < $32 px, else 0. */
#include "xmen2.h"

char Barbarian_TargetClose(Barbarian *self)
{
	long dx = self->target->x - self->x;

	return ABS(dx) < 0x3200;
}
