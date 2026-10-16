/* MondoPlusPowerUp_Collect - MONDOPLUSPOWERUP collected (class slot +$6E): the parent's Collect
 * (the HUD redraw), then the player's health is full ($900, nine segments).
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void MondoPlusPowerUp_Collect(MondoPlusPowerUp *self, register XPlayer *player)
{
	(*CLASS(Tbl_MondoPlusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	player->health = 0x900;
}
