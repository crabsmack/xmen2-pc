/* Boulder_TakeDamage - BOULDER TakeDamage (class slot +$26): a boulder carried by another object
 * (spr.parent, TUSK's) ignores hits; a free one takes them as an ACTOR (whose hurt state is
 * Boulder_StateShatter).
 *
 * self: the boulder.  attacker, type, damage, hx, hy: the hit (passed on). */
#include "xmen2.h"

void Boulder_TakeDamage(register Boulder *self, Actor *attacker, char type, char damage, short hx,
	short hy)
{
	if (self->spr.parent == 0xFF)
		(*CLASS(Tbl_Boulder_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* ACTOR TakeDamage */
}
