/* BabyBio_TakeDamage - BABYBIO TakeDamage (class slot +$26): the parent's (BIONPC) TakeDamage,
 * then, unless that stopped it (flags bit 4), kill its flame.
 *
 * self:     the object.
 * attacker: what hit it.  type: hit type.  damage: health to lose.  hx, hy: the hit point. */
#include "xmen2.h"

void BabyBio_TakeDamage(BabyBio *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	(*CLASS(Tbl_BabyBio_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* BIONPC TakeDamage */
	if (!(self->flags & 0x10) && self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
}
