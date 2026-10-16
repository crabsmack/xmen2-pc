/* SegmentMinusPowerUp_Collect - SEGMENTMINUSPOWERUP collected (class slot +$6E): the parent's
 * Collect (the HUD redraw), then the player loses 1 health segment (its AddHealth, -1).
 *
 * self: the power-up.  player: the player who took it. */
#include "xmen2.h"

void SegmentMinusPowerUp_Collect(SegmentMinusPowerUp *self, XPlayer *player)
{
	(*CLASS(Tbl_SegmentMinusPowerUp_vtable)->parent->method[26])(self, player);	/* POWERUP Collect */
	(*player->cls->method[26])(player, 0, -1);	/* +$6E AddHealth */
}
