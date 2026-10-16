/* SegmentPlusPowerUp_Collect - SEGMENTPLUSPOWERUP collected (class slot +$6E): the parent's
 * Collect (the HUD redraw), then the player gains one health segment (its AddHealth, +1).
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void SegmentPlusPowerUp_Collect(SegmentPlusPowerUp *self, XPlayer *player)
{
	(*CLASS(Tbl_SegmentPlusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	(*player->cls->method[26])(player, 0, 1);	/* +$6E AddHealth */
}
