/* CapedAcolyte_OnHurt - CAPEDACOLYTE OnHurt (class slot +$6E): knocked away by the hit.
 *
 * self: the object.  damage: the hit's damage (unused).  type: hit type: 9-12 the direction
 * it is knocked (9 left, 10 right, 11 down, 12 up), other types don't move it.
 * The state it was in is kept for afterwards (hurtState, unless one is kept already), then
 * CapedAcolyte_StateKnockback runs on the next update. Knocked at $30C with a deceleration of
 * $8E against the motion, in ax for both axes: a vertical knockback brakes x, not y (a bug: vy
 * keeps its speed until CapedAcolyte_StateKnockbackRun ends it). */
#include "xmen2.h"

void CapedAcolyte_OnHurt(register CapedAcolyte *self, char damage, char type)
{
	if (!self->hurtState)
		self->hurtState = self->state;
	self->state = CapedAcolyte_StateKnockback;
	self->timer = 0;
	switch (type) {
	case 12:			/* CapedAcolyte_OnHurt_KnockY */
		self->vy = -0x30C;
		break;
	case 11:
		self->vy = 0x30C;
		break;
	case 9:
		self->vx = -0x30C;
		break;
	case 10:			/* CapedAcolyte_OnHurt_KnockRight */
		self->vx = 0x30C;
		break;
	}
	/* CapedAcolyte_OnHurt_SetDecel */
	if (self->vx > 0)
		self->ax = -0x8E;
	else if (self->vx < 0)
		self->ax = 0x8E;
	if (self->vy > 0)
		self->ax = -0x8E;
	else if (self->vy < 0)
		self->ax = 0x8E;
}
