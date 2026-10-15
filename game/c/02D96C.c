/* Exodus_HurtOnHotTiles - hurt a player standing on EXODUS's energised floor: its TakeDamage
 * (class slot +$26) with no attacker, hit type 4, 1 damage, no hit point.
 *
 * player: g_Player1 or g_Player2 (Exodus_Update).
 * Only when present and alive, its body box bottom below y $304, not in its hit cooldown, and
 * the map block at its position, 12 px up (32-pixel blocks), is one of the scene's 4 extra
 * blocks (g_FgBlockCount .. +3: the hot floor tiles). */
#include "xmen2.h"

void Exodus_HurtOnHotTiles(register XPlayer *player)
{
	register short col, row;
	register short block;

	if (IS_ALIVE(player) && player->boxBottom > 0x304 && !player->hitCooldown) {
		col = player->x >> 13;
		row = (player->y - 0xC00) >> 13;
		block = g_FgBlockMap[g_FgRowTable[row] + col] & 0x1FF;
		if (block >= g_FgBlockCount && block < g_FgBlockCount + 4)
			(*player->cls->method[8])(player, 0, 4, 1, -1, -1);	/* +$26 TakeDamage */
	}
}
