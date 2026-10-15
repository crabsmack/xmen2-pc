/* Exodus_HurtPlayer - hurt a player with EXODUS's psychic flash: its TakeDamage (class slot +$26)
 * with no attacker, hit type 4, 1 damage, no hit point. Not when absent or dead, nor while
 * flagged $1000 unless Player_ClimbState says so.
 *
 * player: g_Player1 or g_Player2 (AvalonA3_PsiFlashStep). */
#include "xmen2.h"

void Exodus_HurtPlayer(register XPlayer *player)
{
	if (IS_ALIVE(player) && (!(player->flags & 0x1000) || (char)Player_ClimbState(player)))
		(*player->cls->method[8])(player, 0, 4, 1, -1, -1);	/* +$26 TakeDamage */
}
