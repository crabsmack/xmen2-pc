/* BanimazaDoor_TakeDamage - BANIMAZADOOR TakeDamage (class slot +$26; the parent's isn't called):
 * the state runs on the next update (delay 0), 20 frames of hit cooldown, remember the attacker and
 * the hit point (x + 16, halfway up the column), health -= damage. Still standing: look 2 (hit).
 * Else: health 4 for the next time, look 3 (shards), 200 frames of cooldown, state
 * BanimazaDoor_Shatter.
 *
 * self: the door.  attacker: who hit it.  damage: health lost.  type, hx, hy: unused. */
#include "xmen2.h"

void BanimazaDoor_TakeDamage(register BanimazaDoor *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	self->timer = 0;
	self->hitCooldown = 20;
	self->attacker = attacker;
	self->hitX = self->px + 16;
	self->hitY = (self->py + (self->y >> 8)) >> 1;
	self->health -= damage;
	if (self->health > 0)
		self->look = 2;
	else {
		self->health = 4;
		self->look = 3;
		self->hitCooldown = 200;
		self->state = BanimazaDoor_Shatter;
	}
}
