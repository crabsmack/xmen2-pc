/* BioBullet_Draw - BIOBULLET Draw (class slot +$1A; the parent's isn't called): frame 0 of its
 * sprite at its screen position (Spr_Draw); with flags bit 5 (frame uploaded) instead one raw 8x8
 * hardware sprite (size 0) 4 px left and 6 px up of it: its first VRAM tile, its palette, high
 * priority when flags bit 2 is set.
 *
 * self: the bullet.
 * The raw sprite is written with no on-screen test. */
#include "xmen2.h"

void BioBullet_Draw(BioBullet *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;
	register short pri;
	register long attr;

	if (!(self->spr.flags & 0x20)) {
		Spr_SetFrame(&self->spr, 0);
		Spr_Draw(&self->spr, sx, sy);
	} else {
		pri = 0;
		if (self->spr.flags & 4)
			pri = (short)0x8000;
		sx -= 4;
		sy -= 6;
		attr = self->spr.tileBase | (self->spr.flags & 3) << 13 | pri;
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
		g_SprWritePtr->y = (short)(sy + 0x80);
		g_SprWritePtr->size = 0;
		g_SprWritePtr->attr = (short)attr;
		g_SprWritePtr->x = (short)(sx + 0x80);
	}
}
