/* MysteryPowerUp_Collect - MYSTERYPOWERUP collected (class slot +$6E): the parent's Collect (the
 * HUD redraw), then the player gains or loses three health segments (its AddHealth, +3 or -3),
 * by bit 1 of g_FrameCounter.
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void MysteryPowerUp_Collect(MysteryPowerUp *self, register XPlayer *player)
{
	register short n = g_FrameCounter & 2 ? 3 : -3;

	(*CLASS(Tbl_MysteryPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	(*player->cls->method[26])(player, 0, n);	/* +$6E AddHealth */
}
