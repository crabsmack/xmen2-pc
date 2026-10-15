/* Tusk_TakeDamage - TUSK TakeDamage (class slot +$26): the parent's (ACTOR), health floored at
 * 0. Still alive: a 90-frame hit cooldown, then a flash (Tusk_HurtFlash) for hit types $D-$F,
 * else a blink and a charge (Tusk_HurtCharge). At 0: an endless cooldown and Tusk_Defeated.
 *
 * self:     TUSK.
 * attacker: what hit it.  type: hit type.  damage: health to lose.  hx, hy: the hit point. */
#include "xmen2.h"

void Tusk_TakeDamage(register Tusk *self, Actor *attacker, register char type, char damage,
	short hx, short hy)
{
	(*CLASS(Tbl_Tusk_vtable)->parent->method[8])(self, attacker, type, damage,
		hx, hy);					/* ACTOR TakeDamage */
	if (self->health < 0)
		self->health = 0;
	if (self->health) {
		self->hitCooldown = 90;
		if (type == 0xD || type == 0xE || type == 0xF)
			self->state = Tusk_HurtFlash;
		else {
			self->flags6 |= 0x40;
			self->state = Tusk_HurtCharge;
		}
	} else {
		self->hitCooldown = 0x7FFF;
		self->state = (StateFn)Tusk_Defeated;
	}
}
