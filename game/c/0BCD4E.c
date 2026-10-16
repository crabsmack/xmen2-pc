/* SavageMediumBio_TakeDamage - SAVAGEMEDIUMBIO TakeDamage (class slot +$26): only BIONPC's
 * TakeDamage (through Tbl_BioNpc_vtable), which SAVAGEBIOBASE inherits anyway: it adds nothing.
 *
 * self: the object.  attacker: what hit it.  type: hit type.  damage: health to lose.
 * hx, hy: the hit point. */
#include "xmen2.h"

void SavageMediumBio_TakeDamage(SavageMediumBio *self, Actor *attacker, char type, char damage,
	short hx, short hy)
{
	(*CLASS(Tbl_BioNpc_vtable)->method[8])(self, attacker, type, damage, hx, hy);	/* BIONPC TakeDamage */
}
