/* CapedAcolyte_OnDeath - CAPEDACOLYTE OnDeath (class slot +$72, XNpc_TakeDamage calls it when a
 * hit takes the last health): hit types 13-15 kill it at once (Kill, slot +$52); any other hit
 * is taken as a hurt (OnHurt, slot +$6E: CapedAcolyte_OnHurt knocks it back, and
 * CapedAcolyte_StateKnockbackRun lets it explode at the end).
 *
 * self: the object.  damage: the hit's damage.  type: hit type. */
#include "xmen2.h"

void CapedAcolyte_OnDeath(register CapedAcolyte *self, char damage, register char type)
{
	if (type == 13 || type == 14 || type == 15)
		(*self->cls->method[19])(self);			/* +$52 Kill */
	else
		(*self->cls->method[26])(self, damage, type);	/* +$6E OnHurt */
}
