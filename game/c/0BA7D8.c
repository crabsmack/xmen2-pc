/* SavageBigBio_TentacleHit - whether a SAVAGEBIGBIO tentacle segment touches a player: the
 * player alive and not in hit cooldown, its second box meeting the 16 x 16 px segment at (x, y).
 * Then its bit goes into g_A4TentacleHits and the segment's centre into *hit (x | y << 16);
 * SavageBigBio_Update hurts it.
 *
 * p: the player.  bit: its bit (1 player 1, 2 player 2).  hit: its hit point (g_A4Hit1/2).
 * x, y: the segment's top left (map pixels). Called for every segment and both players by
 * SavageBigBio_Draw; a later segment's hit point replaces an earlier one's. */
#include "xmen2.h"

#define g_A4TentacleHits	(*(s16 *)g_SceneVar3B06)	/* scene variable: players the tentacle touched this frame (bits 1, 2) */

void SavageBigBio_TentacleHit(register XPlayer *p, short bit, s32 *hit, register short x,
	register short y)
{
	if (IS_ALIVE(p) && !p->hitCooldown && x <= p->box2Right && x + 0x10 >= p->box2Left
	    && y <= p->box2Bottom && y + 0x10 >= p->box2Top) {
		g_A4TentacleHits |= bit;
		*hit = x + 8 | (long)(y + 8) << 16;
	}
}
