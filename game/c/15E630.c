/* Actor_Attack - ACTOR Attack (class slot +$22): hit a target.
 *
 * self: the attacker.  target: what it hit.  type, damage: the hit.  hx, hy: the hit point.
 * The target's TakeDamage (slot +$26) gets the same arguments with self as the attacker; then
 * the attacker's hit cooldown is 10 frames. */
#include "xmen2.h"

void Actor_Attack(Actor *self, Actor *target, char type, char damage, short hx, short hy)
{
	(*target->cls->method[8])(target, self, type, damage, hx, hy);	/* +$26 TakeDamage */
	self->hitCooldown = 10;
}
