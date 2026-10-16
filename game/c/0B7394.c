/* ChildShip_TakeDamage - CHILDSHIP TakeDamage (class slot +$26; the parent's isn't called): a hit
 * with damage while it has health takes the damage and makes it flash 20 frames (no hits); at 0
 * health it is shot down: flashing for good (-1), crashing from the next update
 * (ChildShip_StateCrash), out of the enemy body and attack hit lists (List_Unlink; flags6 |= 1
 * and 2, bits 0 and 1: no attack-list and no body-list entry, +$80/+$84), no bob; else it is
 * knocked back (OnHurt, slot +$6E).
 *
 * self: the object.  attacker: the object that hit it (unused).  type: hit type (9-12: the way
 * it is knocked).  damage: the hit's damage. */
#include "xmen2.h"

void ChildShip_TakeDamage(register ChildShip *self, Actor *attacker, char type, register char damage)
{
	if (damage > 0 && self->health > 0) {
		self->health -= damage;
		self->hitCooldown = 20;
		if (self->health <= 0) {
			self->hitCooldown = -1;
			self->state = ChildShip_StateCrash;
			self->timer = 0;
			List_Unlink(self->bodyNode, &g_HitListEnemyBody);
			self->bodyNode = 0;
			List_Unlink(self->attackNode, &g_HitListEnemyAtk);
			self->attackNode = 0;
			self->flags6 |= 1;
			self->flags6 |= 2;
			self->bob = 0;
		} else
			(*self->cls->method[26])(self, damage, type);	/* +$6E OnHurt */
	}
}
