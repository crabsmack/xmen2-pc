/* FluxBomb_DrawImplode - draw a flux bomb's imploding particles (phase 1): each shown one on screen
 * is an 8x8 hardware sprite of one of the two particle tiles (the last two of g_FluxBombSpr,
 * swapping every 4 frames), palette 1, always in front (the priority test is an or).
 *
 * self: the bomb (FluxBomb_Draw). */
#include "xmen2.h"

#define NEXT_SPRITE()	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))

void FluxBomb_DrawImplode(register FluxBomb *self)
{
	short sx = (self->x >> 8) - g_CameraX;
	short sy = (self->y >> 8) - g_CameraY;
	register FluxParticle *p = self->part;
	register short attr;
	register short tile;
	register short i;
	register short x;
	register short y;

	if (g_FrameCounter >> 2 & 1)
		tile = g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 1;
	else
		tile = g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 2;
	if (g_FluxBombSpr.flags | 4)
		attr = tile | 0x8000 | 0x2000;
	else
		attr = tile | 0x2000;
	for (i = 0; i < 16; i++) {
		if (p->flags & 1) {
			x = sx + p->x;
			y = sy + p->y;
			if (x > -8 && x < 0x140 && y > -8 && y < 0xE0) {
				NEXT_SPRITE();
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = x + 0x80;
			}
		}
		p++;
	}
}
