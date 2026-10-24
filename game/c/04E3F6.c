/* ApocBomb_Draw - APOCBOMB Draw (class slot +$1A; the parent's isn't called): unless its frame is
 * $FF, two hardware sprites from its layout table (+$A0): the bomb at its position and, only
 * when the bomb is on screen, its shadow (shadowFrame) at the ground under it (groundY), when
 * that is on screen too.
 *
 * self: the bomb.
 * A frame's sprite is drawn dx left and dy above the point; on screen means x > -2 dx, x < 320,
 * y > -dy, y < 224. Tiles from the sprite's first VRAM tile, palette from its flags, high
 * priority with flags bit 2; the bomb also takes the h-flip (flags bit 4), the shadow doesn't.
 * Only the bomb's attr, both flags and the shadow's x are `register`: the rest compete by use
 * count, which gives the ROM's register order. */
#include "xmen2.h"

void ApocBomb_Draw(register ApocBomb *self)
{
	register SprInst *spr = &self->spr;
	unsigned char frame = spr->frame;

	if (frame != 0xFF) {
		ApocBombFrame *f = self->frames + frame;
		short dx = f->dx;
		short dy = f->dy;
		short x = (self->x >> 8) - g_CameraX - dx;
		short y = (self->y >> 8) - g_CameraY - dy;

		if (x > VIEW_L(-(dx << 1)) && x < VIEW_R(320) && y > -dy && y < 224) {
			register short attr = spr->tileBase + f->tile;
			char size = f->size;
			register unsigned char flags = spr->flags;

			attr |= (flags & 3) << 13;
			attr |= flags & 4 ? 0x8000 : 0;
			attr |= flags & 0x10 ? 0x800 : 0;
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = size;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = x + 0x80;
			{
				ApocBombFrame *f = self->frames + self->shadowFrame;
				short dx = f->dx;
				short dy = f->dy;
				register short x = (self->x >> 8) - g_CameraX - dx;
				short y = self->groundY - g_CameraY - dy;

				if (x > VIEW_L(-(dx << 1)) && x < VIEW_R(320) && y > -dy && y < 224) {
					short attr = spr->tileBase + f->tile;
					char size = f->size;
					register unsigned char flags = spr->flags;

					attr |= (flags & 3) << 13;
					attr |= flags & 4 ? 0x8000 : 0;
					g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
					g_SprWritePtr->y = y + 0x80;
					g_SprWritePtr->size = size;
					g_SprWritePtr->attr = attr;
					g_SprWritePtr->x = x + 0x80;
				}
			}
		}
	}
}
