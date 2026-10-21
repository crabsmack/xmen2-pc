/* ExplodeFx_TrailDraw - a Trail's draw-list hook: each dropped puff (1 to the head's count) that
 * is live and on the screen (-32 < x < 320, -32 < y < 224) as one 32x32 hardware sprite (size $F)
 * of the shared explosion tiles (g_FxTileBase + frame * 16), palette 1, high priority when the
 * effect has it (flags bit 0). The head itself isn't drawn.
 *
 * fx: the effect. */
#include "xmen2.h"

void ExplodeFx_TrailDraw(register ExplodeFx *fx)
{
	FxPuff *head;
	short frame;
	register FxPuff *p;
	register short i;
	register short attr;
	register short sx, sy;

	i = 0;
	p = (FxPuff *)(fx + 1);
	head = p;
	p++;
	i++;
	for (;; i++, p++) {
		if (i >= head->type)
			break;
		if (p->type) {
			sx = p->x >> 8;
			sy = p->y >> 8;
			frame = p->anim >> 4;
			sx -= g_CameraX;
			sy -= g_CameraY;
			attr = g_FxTileBase + (frame << 4);
			attr |= 0x2000;
			attr |= fx->flags & 1 ? 0x8000 : 0;
			if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy > -32 && sy < 224) {
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0xF;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			}
		}
	}
}
