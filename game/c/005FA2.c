/* Player_SetCharacter - make a player object another character (class), where it is.
 *
 * cls:    the character's class descriptor (Tbl_Beast_vtable...).
 * player: g_Player1 or g_Player2.
 * The position: its own if it is alive (then it is killed first, slot +$52), else the other
 * player's if that one is alive, else the screen centre ($A0, $6E from the camera). In the level
 * loop (g_InLevelLoop) it is constructed there at once (Init, slot +$12, no placement), health
 * $700, activated (slot +$3E), sprite priority (bit 2) set if the other, alive, has it. Its pad:
 * 0 for player 1, 1 for player 2. */
#include "xmen2.h"

void Player_SetCharacter(Class *cls, register XPlayer *player)
{
	register XPlayer *other;
	register long x, y;

	other = player == &g_Player1 ? &g_Player2 : &g_Player1;
	if (IS_ALIVE(player)) {
		x = player->x;
		y = player->y;
		(*player->cls->method[19])(player);	/* +$52 Kill */
		player->cls = cls;
	} else {
		player->cls = cls;
		if (IS_ALIVE(other)) {
			x = other->x;
			y = other->y;
		} else {
			x = g_CameraX + 0xA0 << 8;
			y = g_CameraY + 0x6E << 8;
		}
	}
	if (g_InLevelLoop) {
		(*player->cls->method[3])(player, x, y, 0);	/* +$12 Init */
		player->health = 0x700;
		(*player->cls->method[14])(player);		/* +$3E Activate */
		if (IS_ALIVE(other) && (other->spr.flags & 4))
			player->spr.flags |= 4;
	}
	if (player == &g_Player1)
		player->pad = 0;
	else
		player->pad = 1;
}
