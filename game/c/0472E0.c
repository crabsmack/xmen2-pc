/* BaniMazaA2_UpdateDebrisGroup - one frame of a debris group while its timer runs (1 off, 2 in
 * two-player step mode): bits 0-11 are drawn (one 8x8 hardware sprite each, on every other call
 * of a bit: g_A2DebrisFrame counts them), moved by (dx, dy) and pulled down (dy + 1); in
 * two-player step mode all 16 bits move once more.
 *
 * g: the group. Never runs in the corpus (nothing starts a group). */
#include "xmen2.h"

#define g_A2DebrisFrame	(*(s32 *)g_SceneVar3B18)	/* scene variable: debris bits handled so far (bit 0: draw) */

void BaniMazaA2_UpdateDebrisGroup(register DebrisGroup *g)
{
	register DebrisBit *p;
	register short i;

	if (g->timer > 0) {
		g->timer -= g_TwoPlayerStep ? 2 : 1;
		p = g->bit;
		for (i = 4; i < 16; i++) {
			if (g_A2DebrisFrame & 1) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = p->y - g_CameraY + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = p->attr;
				g_SprWritePtr->x = p->x - g_CameraX + 0x80;
			}
			g_A2DebrisFrame++;
			p->x += p->dx;
			p->y += p->dy;
			++p->dy;
			p++;
		}
		if (g_TwoPlayerStep) {
			p = g->bit;
			for (i = 0; i < 16; i++) {
				p->x += p->dx;
				p->y += p->dy;
				++p->dy;
				p++;
			}
		}
	}
}
