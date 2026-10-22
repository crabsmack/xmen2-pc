/* Spr_InitLinks - chain the 80 hardware sprite slots and reset the sprite list.
 *
 * Each slot of g_SpriteTable links to the next (link = index + 1); the two entries after the 80
 * slots are the list's sentinels (80 links to itself, 81 ends with 0; Spr_BeginFrame starts
 * writing at 81). No slot was used last frame (g_SprLast $51). */
#include "xmen2.h"

void Spr_InitLinks(void)
{
	register short i;

	for (i = 0; i < SPR_STOP; i++)		/* SPR_STOP, SPR_START: 80, $51 (phase 7: raised, xmen2_mem.h) */
		g_SpriteTable[i].link = i + 1;
	g_SpriteTable[SPR_STOP].link = SPR_STOP;
	g_SpriteTable[SPR_START].link = 0;
	g_SprUnusedE224 = 0;
	g_SprLast = SPR_START;
	g_SprUnusedE220 = 0;
}
