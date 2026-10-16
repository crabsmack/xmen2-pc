/* ExplodeFx_ChainDraw - a chain's draw-list hook: each live explosion on the screen (-32 < x <
 * 320, -32 < y < 224) as one 32x32 hardware sprite (size $F) of the shared explosion tiles
 * (g_FxTileBase + frame * 16), palette 1, high priority when the effect has it (flags bit 0).
 *
 * fx: the chain. */
#include "xmen2.h"

void ExplodeFx_ChainDraw(register ExplodeFxChain *fx)
{
	short frame;
	register FxBlast *b;
	register short i;
	register short attr;
	register short sx, sy;

	i = 0;
	b = (FxBlast *)(fx + 1);
	for (;; i++, b++) {
		if (i >= 27)
			break;
		if (b->live) {
			sx = b->x;
			sy = b->y;
			frame = b->anim >> 4;
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
