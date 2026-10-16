/* CoreZap_HitActor - hit an object with the zap: if it is alive, not flashing from a hit and its
 * second box overlaps the zap's, 1 damage of hit type 4 at the zap's position (pixels), with no
 * attacker (0), through its TakeDamage (class slot +$26).
 *
 * self: the zap.  target: the object (CoreZap_Update: each player). */
#include "xmen2.h"

void CoreZap_HitActor(register CoreZap *self, register Actor *target)
{
	if (IS_ALIVE(target) && !target->hitCooldown
	    && self->box2Left <= target->box2Right && self->box2Right >= target->box2Left
	    && self->box2Top <= target->box2Bottom && self->box2Bottom >= target->box2Top)
		(*target->cls->method[8])(target, 0, 4, 1, self->x >> 8, self->y >> 8);	/* +$26 TakeDamage: no attacker */
}
