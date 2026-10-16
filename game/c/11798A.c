/* XPowerPlusPowerUp_Collect - XPOWERPLUSPOWERUP collected (class slot +$6E): the parent's Collect
 * (the HUD redraw), then the player gains 3 health segments (its AddHealth, +3).
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void XPowerPlusPowerUp_Collect(XPowerPlusPowerUp *self, XPlayer *player)
{
	(*CLASS(Tbl_XPowerPlusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	(*player->cls->method[26])(player, 0, 3);	/* +$6E AddHealth */
}
