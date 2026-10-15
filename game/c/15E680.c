/* Actor_TakeDamage - ACTOR TakeDamage (class slot +$26): be hit.
 *
 * self: the object.  attacker: who hit it.  type, damage: the hit.  hx, hy: the hit point.
 * With a hurt state, it becomes the state now (timer 0); 10 frames of hit cooldown; health -=
 * damage. */
#include "xmen2.h"

void Actor_TakeDamage(Actor *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	if (self->hurtState) {
		self->state = self->hurtState;
		self->timer = 0;
	}
	self->hitCooldown = 10;
	self->health -= damage;
}
