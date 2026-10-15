/* Barbarian_TakeDamage - BARBARIAN TakeDamage (class slot +$26): remember flags6 as it is now
 * (hurtFlags6: Barbarian_EndHurt restores its bits $5000), then the parent's (BIONPC)
 * TakeDamage.
 *
 * self:     the object.
 * attacker: what hit it.  type: hit type.  damage: health to lose.  hx, hy: the hit point. */
#include "xmen2.h"

void Barbarian_TakeDamage(Barbarian *self, Actor *attacker, char type, char damage, short hx,
	short hy)
{
	self->hurtFlags6 = self->flags6;
	(*CLASS(Tbl_Barbarian_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* BIONPC TakeDamage */
}
