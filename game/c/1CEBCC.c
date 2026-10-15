/* Magneto_TakeDamage - MAGNETO TakeDamage (class slot +$26): the player's (XPLAYER) TakeDamage,
 * and a flux bomb he holds is let go (FluxBomb_StateHeld then implodes it).
 *
 * self: the player.  attacker, type, damage, hx, hy: as XPlayer_TakeDamage (passed on). */
#include "xmen2.h"

void Magneto_TakeDamage(Magneto *self, Actor *attacker, char type, char damage, short hx,
	short hy)
{
	(*CLASS(Tbl_Magneto_vtable)->parent->method[8])(self, attacker, type, damage,
		hx, hy);					/* XPLAYER TakeDamage */
	if (self->bomb)
		self->bomb->held = 0;
}
