/* FloatBubble_Attack - FLOATBUBBLE Attack (class slot +$22): the parent's (ACTOR: the target's
 * TakeDamage), then 1 health and burst (FloatBubble_StateBurst, also as default state): a bubble
 * that hits someone bursts.
 *
 * self: the bubble.  target: who it hits.  type, damage: the hit (bytes).
 * hx, hy: the hit point. Never runs in the corpus. */
#include "xmen2.h"

void FloatBubble_Attack(FloatBubble *self, Actor *target, char type, char damage, short hx, short hy)
{
	(*CLASS(Tbl_FloatBubble_vtable)->parent->method[7])(self, target, type, damage, hx, hy);
	self->health = 1;
	self->state = self->defaultState = (StateFn)FloatBubble_StateBurst;
}
