/* ApocBomb_Draw - APOCBOMB Draw (class slot +$1A; the parent's isn't called): unless its frame is
 * $FF, two hardware sprites from its layout table (+$A0): the bomb at its position and, only
 * when the bomb is on screen, its shadow (shadowFrame) at the ground under it (groundY), when
 * that is on screen too.
 *
 * self: the bomb.
 * A frame's sprite is drawn dx left and dy above the point; on screen means x > -2 dx, x < 320,
 * y > -dy, y < 224. Tiles from the sprite's first VRAM tile, palette from its flags, high
 * priority with flags bit 2; the bomb also takes the h-flip (flags bit 4), the shadow doesn't.
 * (Not byte-matched: the ROM keeps both halves' locals apart on the stack, declared in an
 * unusual order, and its second half's flags copy lives on the stack; executed, so verified by
 * the shadow test.) */
#include "xmen2.h"

void ApocBomb_Draw(register ApocBomb *self)
{
	register SprInst *spr = &self->spr;
	register unsigned char frame = spr->frame;

	if (frame != 0xFF) {
		char size;
		ApocBombFrame *f = self->frames + frame;
		register short dx = f->dx;
		register short dy = f->dy;
		register short x = (self->x >> 8) - g_CameraX - dx;
		register short y = (self->y >> 8) - g_CameraY - dy;

		if (x > -(dx << 1) && x < 320 && y > -dy && y < 224) {
			register short attr = spr->tileBase + f->tile;
			register unsigned char flags;

			size = f->size;
			flags = spr->flags;
			attr |= (flags & 3) << 13;
			attr |= flags & 4 ? 0x8000 : 0;
			attr |= flags & 0x10 ? 0x800 : 0;
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = size;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = x + 0x80;
			{
				char size;
				ApocBombFrame *f = self->frames + self->shadowFrame;
				register short dx = f->dx;
				register short dy = f->dy;
				register short x = (self->x >> 8) - g_CameraX - dx;
				register short y = self->groundY - g_CameraY - dy;

				if (x > -(dx << 1) && x < 320 && y > -dy && y < 224) {
					register short attr = spr->tileBase + f->tile;
					register unsigned char flags;

					size = f->size;
					flags = spr->flags;
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
