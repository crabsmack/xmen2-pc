/* SavageA4_InitStats - fill the stats record the Chamber's enemies share (g_A4Stats, in
 * SavageA4_RunScene's frame): attack 2, hurt sound 0 (the default), health 2 (the Inits set their
 * own), knockback 5, 10, 15 and 30 px, no hurt frame; then the boss's health as the frame hook
 * sees it 50 (both values: no palette stage yet) and no tentacle hits. */
#include "xmen2.h"

#define g_A4Stats		(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4BossHealth		(*(s16 *)g_SceneVar3B02)	/* scene variable: SAVAGEBIGBIO's health (SavageBigBio_TakeDamage) */
#define g_A4BossHealthSeen	(*(s16 *)g_SceneVar3B04)	/* scene variable: its health at the last frame hook */
#define g_A4TentacleHits	(*(s16 *)g_SceneVar3B06)	/* scene variable: players the tentacle touched this frame (bits 1, 2) */

void SavageA4_InitStats(void)
{
	g_A4Stats->attack = 2;
	g_A4Stats->hurtSound = 0;
	g_A4Stats->health = 2;
	g_A4Stats->knockDist[0] = 5;
	g_A4Stats->knockDist[1] = 10;
	g_A4Stats->knockDist[2] = 15;
	g_A4Stats->knockDist[3] = 30;
	g_A4Stats->hurtFrame = 0xFF;
	g_A4BossHealth = 50;
	g_A4BossHealthSeen = g_A4BossHealth;
	g_A4TentacleHits = 0;
}
