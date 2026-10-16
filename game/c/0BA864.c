/* SavageBigBio_Update - SAVAGEBIGBIO Update (class slot +$4A): the parent's (SAVAGEBIOBASE)
 * Update, then each player the tentacle touched this frame (g_A4TentacleHits, set by
 * SavageBigBio_TentacleHit from Draw) takes TakeDamage (class slot +$26) from no attacker, hit
 * type 8, 1 damage, at its hit point (g_A4Hit1 / g_A4Hit2: x low word, y high word).
 *
 * self: the boss. */
#include "xmen2.h"

#define g_A4TentacleHits	(*(s16 *)g_SceneVar3B06)	/* scene variable: players the tentacle touched this frame (bits 1, 2) */
#define g_A4Hit1		(*(s32 *)g_SceneVar3B18)	/* scene variable: where the tentacle touched player 1: x | y << 16 */
#define g_A4Hit2		(*(s32 *)g_SceneVar3B1C)	/* scene variable: the same for player 2 */

void SavageBigBio_Update(SavageBigBio *self)
{
	(*CLASS(Tbl_SavageBigBio_vtable)->parent->method[17])(self);	/* SAVAGEBIOBASE Update */
	if (g_A4TentacleHits & 1)
		(*g_Player1.cls->method[8])(&g_Player1, 0, 8, 1, g_A4Hit1 & 0xFFFF, g_A4Hit1 >> 16);	/* +$26 TakeDamage */
	if (g_A4TentacleHits & 2)
		(*g_Player2.cls->method[8])(&g_Player2, 0, 8, 1, g_A4Hit2 & 0xFFFF, g_A4Hit2 >> 16);	/* +$26 TakeDamage */
}
