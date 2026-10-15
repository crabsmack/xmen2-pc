/* Brood_TakeDamage - BROOD TakeDamage (class slot +$26): remember the hit, turn to the attacker
 * and charge (Brood_ActCharge, which dodges instead after a hit), then the parent's
 * (SMALLBROOD) TakeDamage and 60 frames of hit cooldown.
 *
 * self: the object.  attacker: who hit it.  type, damage: the hit (bytes).
 * hx, hy: the hit point. */
#include "xmen2.h"

void Brood_TakeDamage(Brood *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	self->aiFlags |= 1;
	if (attacker->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->state = Brood_ActCharge;
	self->timer = 0;
	(*CLASS(Tbl_Brood_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);
	self->hitCooldown = 60;
}
