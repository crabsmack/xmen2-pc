/* FloatBubble_TakeDamage - FLOATBUBBLE TakeDamage (class slot +$26): the parent's (ACTOR), then
 * whatever the damage, 1 health left and burst (FloatBubble_StateBurst, also as default state).
 *
 * self: the bubble.  attacker: who hit it.  type, damage: the hit (bytes).
 * hx, hy: the hit point. */
#include "xmen2.h"

void FloatBubble_TakeDamage(FloatBubble *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	(*CLASS(Tbl_FloatBubble_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);
	self->health = 1;
	self->state = self->defaultState = (StateFn)FloatBubble_StateBurst;
}
