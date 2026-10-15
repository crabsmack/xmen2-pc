/* FlyBrood_Draw - FLYBROOD Draw (class slot +$1A): one 32x24 hardware sprite of its own (size
 * $E) from its sprite set g_FlyBroodSprDef (12 tiles a frame, palette 1, h-flip and priority
 * from the sprite flags), its top left 16 px left of and 24 px above the position; nothing when
 * no frame is shown. In the raw-art scene (g_ScnUnused3B5C) the parent's (ACTOR) Draw instead.
 *
 * self: the object.
 * (It is drawn wherever it is: no on-screen test.) */
#include "xmen2.h"

void FlyBrood_Draw(FlyBrood *self)
{
	if (g_ScnUnused3B5C != g_SceneIndex) {
		short attr = g_SpriteDefs[g_FlyBroodSprDef].tileBase | 0x2000;
		short sx = (self->x >> 8) - 0x10 - g_CameraX;
		short sy = (self->y >> 8) - 0x18 - g_CameraY;
		SprInst *spr = &self->spr;
		short frame;

		frame = spr->frame;
		if (frame && 0) {
			/* compiled out; MPW keeps the test of the frame (tst.l) */
		}
		if (frame != 0xFF) {
			register char flags = spr->flags;
			register long t;

			if (flags & 0x10)
				attr |= 0x800;
			if (flags & 4)
				attr |= 0x8000;
			attr += (short)(t = frame, (t << 3) + (t << 2));
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xE;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = sx + 0x80;
		}
	} else
		(*CLASS(Tbl_FlyBrood_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
