/* XPowerMinusPowerUp_Collect - XPOWERMINUSPOWERUP collected (class slot +$6E): the parent's
 * Collect (the HUD redraw), then the player loses 3 health segments (its AddHealth, -3).
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void XPowerMinusPowerUp_Collect(XPowerMinusPowerUp *self, XPlayer *player)
{
	(*CLASS(Tbl_XPowerMinusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	(*player->cls->method[26])(player, 0, -3);	/* +$6E AddHealth */
}
