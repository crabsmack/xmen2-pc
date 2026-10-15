/* Spr_InitLinks - chain the 80 hardware sprite slots and reset the sprite list.
 *
 * Each slot of g_SpriteTable links to the next (link = index + 1); the two entries after the 80
 * slots are the list's sentinels (80 links to itself, 81 ends with 0; Spr_BeginFrame starts
 * writing at 81). No slot was used last frame (g_SprLast $51). */
#include "xmen2.h"

void Spr_InitLinks(void)
{
	register short i;

	for (i = 0; i < 80; i++)
		g_SpriteTable[i].link = i + 1;
	g_SpriteTable[80].link = 80;
	g_SpriteTable[81].link = 0;
	g_SprUnusedE224 = 0;
	g_SprLast = 0x51;
	g_SprUnusedE220 = 0;
}
