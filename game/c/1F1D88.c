/* XPlayer_AddHealth - XPLAYER class slot +$6E: add whole health units, keeping health in 1..$900.
 *
 * self:   the player.
 * unused: not read.
 * units:  units to add ($100 of health each; negative takes away).
 * Marks the HUD for redraw. Nothing happens when units << 8 is 0 as a word. */
#include "xmen2.h"

void XPlayer_AddHealth(XPlayer *self, long unused, short units)
{
	short add;

	if (add = units << 8) {
		short health = self->health + add;

		self->health = health < 1 ? 1 : health > 0x900 ? 0x900 : health;
		g_HudDirty = 1;
	}
}
