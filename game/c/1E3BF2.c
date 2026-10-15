/* Cyclops_TakeDamage - CYCLOPS TakeDamage (class slot +$26): the parent's (XPLAYER,
 * XPlayer_TakeDamage), and a beam that is out ends (Cyclops_EndBeam).
 *
 * self: the player.  attacker, type, damage, hx, hy: as XPlayer_TakeDamage (passed on). */
#include "xmen2.h"

void Cyclops_TakeDamage(register Cyclops *self, Actor *attacker, char type, char damage, short hx,
	short hy)
{
	(*CLASS(Tbl_Cyclops_vtable)->parent->method[8])(self, attacker, type, damage,
		hx, hy);					/* XPLAYER TakeDamage */
	if (self->beam.def)
		Cyclops_EndBeam(self);
}
