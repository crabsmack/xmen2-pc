/* Player_MusicVoice - the music voice of a player's character (Music_SetPlayerParams): 1 Beast,
 * 2 Nightcrawler, 3 Magneto, 4 Gambit, 5 Wolverine, 6 Cyclops, 7 Psylocke; 0 for a player not
 * in play (or any other class).
 *
 * player: g_Player1 or g_Player2. */
#include "xmen2.h"

long Player_MusicVoice(register XPlayer *player)
{
	if (!IS_ALIVE(player))
		return 0;
	if (player->cls == CLASS(Tbl_Beast_vtable))
		return 1;
	if (player->cls == CLASS(Tbl_Nightcrawler_vtable))
		return 2;
	if (player->cls == CLASS(Tbl_Magneto_vtable))
		return 3;
	if (player->cls == CLASS(Tbl_Gambit_vtable))
		return 4;
	if (player->cls == CLASS(Tbl_Wolverine_vtable))
		return 5;
	if (player->cls == CLASS(Tbl_Cyclops_vtable))
		return 6;
	if (player->cls == CLASS(Tbl_Psylocke_vtable))
		return 7;
	return 0;
}
