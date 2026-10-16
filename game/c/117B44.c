/* MondoMinusPowerUp_Collect - MONDOMINUSPOWERUP collected (class slot +$6E): the parent's Collect
 * (the HUD redraw), then the player's health is cut to at most $300 (three segments).
 *
 * self: the power-up.  player: the player who took it.
 * The health is read before the parent's Collect. */
#include "xmen2.h"

void MondoMinusPowerUp_Collect(MondoMinusPowerUp *self, register XPlayer *player)
{
	register short health = player->health;
	register short max = 0x300;

	(*CLASS(Tbl_MondoMinusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	if (health > max)
		player->health = max;
}
