/* FluxBomb_DrawBurst - draw a flux bomb's bursting particles (phase 3): each shown one of the
 * first burstCount (at most 16) on screen is two 8x8 hardware sprites, the particle (the last
 * particle tile of g_FluxBombSpr) and a trail one step back along its direction (the one before),
 * palette 1, always in front (the priority test is an or).
 *
 * self: the bomb (FluxBomb_Draw). */
#include "xmen2.h"

#define NEXT_SPRITE()	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))

void FluxBomb_DrawBurst(FluxBomb *self)
{
	register short prio = g_FluxBombSpr.flags | 4 ? 0x8000 : 0;
	short attr1 = (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 1 | prio) | 0x2000;
	short attr2 = (g_FluxBombSpr.tileBase + g_FluxBombSpr.tileCount - 2 | prio) | 0x2000;
	short sx = (self->x >> 8) - g_CameraX;
	short sy = (self->y >> 8) - g_CameraY;
	register FluxParticle *p = self->part;
	register short n = MIN(self->burstCount, 16);
	register FluxDir *d = self->strong ? (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs)
		: (FluxDir *)CODEPTR(Tbl_FluxBombBurstDirs);
	register short i;
	register short x;
	register short y;

	for (i = 0; i < n; i++) {
		if (p->flags & 1) {
			x = sx + p->x;
			y = sy + p->y;
			if (x > -8 && x < 0x140 && y > -8 && y < 0xE0) {
				NEXT_SPRITE();
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr1;
				g_SprWritePtr->x = x + 0x80;
				x -= d->dx;
				y -= d->dy;
				NEXT_SPRITE();
				g_SprWritePtr->y = y + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr2;
				g_SprWritePtr->x = x + 0x80;
			}
		}
		p++;
		d++;
	}
}
