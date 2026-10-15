/* Exodus_DropPlayerFromSeg - make a player standing on floor segment seg fall (Obj_Detach):
 * only when present and alive and not flagged $1000.
 *
 * player: g_Player1 or g_Player2.  seg: the floor segment that breaks (Exodus_CheckFloorHit). */
#include "xmen2.h"

void Exodus_DropPlayerFromSeg(register XPlayer *player, Seg *seg)
{
	if (IS_ALIVE(player) && !(player->flags & 0x1000) && player->seg == seg)
		Obj_Detach(player);
}
