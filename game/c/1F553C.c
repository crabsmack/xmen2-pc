/* Map_SetRowPalette - give the 16 tile words (4 x 4 tiles) of one block definition another
 * palette line (CloneA1_InitSprites, SentinelsA1_InitZapLights, SiberiaB1_InitWindSprite).
 *
 * defs:  the block definitions (g_FgBlockDefs: 16 words each).
 * count: the level's block count (g_FgBlockCount).
 * n:     which block after them: block count + n - 1.
 * pal:   the palette line, 0-3 (tile bits 13-14). */
#include "xmen2.h"

void Map_SetRowPalette(void *defs, short count, short n, register short pal)
{
	s16 *p;
	register short i;

	p = (s16 *)defs + ((count + n - 1) << 4);
	pal = (pal & 3) << 13;
	for (i = 0; i < 16; i++) {
		*p = (*p & ~0x6000) | pal;
		p++;
	}
}
