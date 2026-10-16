/* ExplodeFx_BounceDraw - a Bounce's draw-list hook: each released, moving puff of the 5 groups on
 * the screen (-32 < x < 320, -32 < y < 224) as one 32x32 hardware sprite (size $F) of the shared
 * explosion tiles (g_FxTileBase + frame * 16), palette 1, high priority when the effect has it
 * (flags bit 0).
 *
 * fx: the effect. */
#include "xmen2.h"

void ExplodeFx_BounceDraw(register ExplodeFx *fx)
{
	short n;
	short frame;
	register FxGroup *g;
	short j;
	short attr;
	register short sx, sy;

	n = 0;
	g = (FxGroup *)(fx + 1);
	for (;; n++, g++) {
		if (n >= 5)
			break;
		if (!g->count)
			continue;
		for (j = 0; j < g->count; j++) {
			if (!g->vx[j] && !g->vy[j])
				continue;
			sx = g->x[j] >> 3;
			sy = g->y[j] >> 3;
			frame = g->anim[j] >> 4;
			sx -= g_CameraX;
			sy -= g_CameraY;
			attr = g_FxTileBase + (frame << 4);
			attr |= 0x2000;
			attr |= fx->flags & 1 ? 0x8000 : 0;
			if (sx > -32 && sx < 320 && sy > -32 && sy < 224) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0xF;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			}
		}
	}
}
