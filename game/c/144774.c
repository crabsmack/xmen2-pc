/* ToxicCan_DrawLeak - draw the toxin leaking from a damaged TOXICCAN: four 8x8 puffs of the
 * g_BioFlameSprDef sprite set's tiles, animated by two step counters through the puff tables.
 *
 * x, y:  the can's position (world pixels; y its body box top).
 * step1: the first animation's step (Tbl_ToxicCanLeakPuffs1: three puffs, 10 px either side
 *        and one in the middle).
 * step2: the second's (Tbl_ToxicCanLeakPuffs2: one puff).
 * Both steps advance by 1 (2 in two-player step mode) and wrap at the table's end (a tile
 * >= $FF). Nothing without a valid sprite set. Each puff takes the next hardware sprite slot
 * (no on-screen test), palette and tiles from the set, in front. */
#include "xmen2.h"

#define NEXT_SPRITE()	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))

void ToxicCan_DrawLeak(register short x, register short y, s16 *step1, s16 *step2)
{
	SprInst *def;
	register short attr;
	register LeakPuff *p;
	register LeakPuff *q;

	if (g_BioFlameSprDef >= 0 && g_BioFlameSprDef < g_SpriteDefCount) {
		def = &g_SpriteDefs[g_BioFlameSprDef];
		x -= g_CameraX;
		y -= g_CameraY;
		if (g_TwoPlayerStep) {
			*step1 += 2;
			*step2 += 2;
		} else {
			(*step1)++;
			(*step2)++;
		}
		p = (LeakPuff *)CODEPTR(Tbl_ToxicCanLeakPuffs1);
		q = (LeakPuff *)CODEPTR(Tbl_ToxicCanLeakPuffs2);
		p += *step1;
		q += *step2;
		if (p->tile >= 0xFF) {
			*step1 = 0;
			p = (LeakPuff *)CODEPTR(Tbl_ToxicCanLeakPuffs1);
		}
		if (q->tile >= 0xFF) {
			*step2 = 0;
			q = (LeakPuff *)CODEPTR(Tbl_ToxicCanLeakPuffs2);
		}
		attr = def->tileBase | (def->flags & 3) << 13;
		attr |= 0x8000;
		/* (the (short) casts give the original's mix of long and word arithmetic) */
		NEXT_SPRITE();
		g_SprWritePtr->y = (short)y + p->dy + 0x80;
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = attr + p->tile;
		g_SprWritePtr->x = (short)x + p->dx + 0x80;
		NEXT_SPRITE();
		g_SprWritePtr->y = (short)(y + p->dy + 5) + 0x80;
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = attr + p->tile;
		g_SprWritePtr->x = (short)(x - p->dx) - 10 + 0x80;
		NEXT_SPRITE();
		g_SprWritePtr->y = (short)(y + p->dy + 5) + 0x80;
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = attr + p->tile;
		g_SprWritePtr->x = (short)(x - p->dx) + 10 + 0x80;
		NEXT_SPRITE();
		g_SprWritePtr->y = (short)(y + q->dy + 2) + 0x80;
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = attr + q->tile;
		g_SprWritePtr->x = (short)(x - q->dx) - 7 + 0x80;
	}
}
