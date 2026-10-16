/* PowerUp_Collect - POWERUP's Collect (class slot +$6E; every subclass's Collect calls it first):
 * the HUD is redrawn (g_HudDirty).
 *
 * self: the power-up.  player: the player who took it (unused here). */
#include "xmen2.h"

void PowerUp_Collect(PowerUp *self, XPlayer *player)
{
	g_HudDirty = 1;
}
