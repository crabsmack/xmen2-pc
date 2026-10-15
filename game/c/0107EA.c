/* BigAcolyte_TakeDamage - BIGACOLYTE TakeDamage (class slot +$26).
 *
 * self:     the boss.
 * attacker: what hit it.  type: hit type.  damage: health to lose.  hx, hy: the hit point.
 * Outside its hit cooldown: sound $D8, its speed limit grows by $1C per health point lost
 * before this hit ($280 at full health), the damage is taken; hit types $D-$F while it is a
 * sprite and still alive go to the parent's (BIONPC) TakeDamage, anything else gives a
 * 40-frame blink. At 0 health (whether or not it was hit now): health 0, an endless cooldown
 * and the death state BigAcolyte_DieEndScene (also the default state). */
#include "xmen2.h"

void BigAcolyte_TakeDamage(register BigAcolyte *self, Actor *attacker, register char type,
	register char damage, short hx, short hy)
{
	short lost = 24 - self->health;
	register short speed;

	if (!self->hitCooldown) {
		Snd_Stop(0xD8);
		Snd_Play(0xD8);
		speed = lost * 0x1C + 0x280;
		self->maxVy = speed;
		self->minVy = -speed;
		self->health -= damage;
		if ((type == 0xD || type == 0xE || type == 0xF) && !self->picture && self->health > 0)
			(*CLASS(Tbl_BigAcolyte_vtable)->parent->method[8])(self, attacker, type, damage,
				hx, hy);					/* BIONPC TakeDamage */
		else {
			self->hitCooldown = 40;
			self->flags6 |= 0x40;
		}
	}
	if (self->health <= 0) {
		self->health = 0;
		self->hitCooldown = 0x7FFF;
		self->timer = 0;
		self->state = self->defaultState = (StateFn)BigAcolyte_DieEndScene;
	}
}
