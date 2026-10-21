/* FarAcolyte_Draw - FARACOLYTE Draw (class slot +$1A): its orb, its bolt, then itself.
 *
 * self: the acolyte.
 * The orb (while out) is two 32x32 hardware sprites of the Art_FarAcolyteSpr3 tiles, the left
 * one h-flipped, 16 px left of and above the orb's position, high priority, palette 1, or
 * palettes 1/2 on alternate frames once it has reached the player; each is emitted only when
 * on screen. The bolt (while out) is its sprite definition (Art_FarAcolyteSpr2) at its
 * animation frame. The acolyte itself: the parent's (ACTOR) Draw.
 * The acolyte's own screen position is computed first and never used. */
#include "xmen2.h"

void FarAcolyte_Draw(register FarAcolyte *self)
{
	short x = (self->x >> 8) - g_CameraX;
	short y = (self->y >> 8) - g_CameraY;
	if (self->shotFlags & 1) {
		register FarAcolyteShot *s = &self->orb;
		short sx = (s->x >> 8) - g_CameraX - 16;
		short sy = (s->y >> 8) - g_CameraY - 16;
		short attr = g_SpriteDefs[g_FarAcolyteSprDef3].tileBase | 0x8000;

		if (self->shotFlags & 2) {
			if (g_FrameCounter & 1)
				attr |= 0x2000;
			else
				attr |= 0x4000;
		} else
			attr |= 0x2000;
		if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 7;
			g_SprWritePtr->attr = attr | 0x800;
			g_SprWritePtr->x = sx + 0x80;
		}
		if (sx + 15 > VIEW_L(-32) && sx + 15 < VIEW_R(320) && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 7;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = sx + 0x8F;
		}
	}
	if (self->shotFlags & 4) {
		register FarAcolyteShot *s = &self->bolt;
		SprInst *def = &g_SpriteDefs[g_FarAcolyteSprDef2];
		short bx = (s->x >> 8) - g_CameraX;
		short by = (s->y >> 8) - g_CameraY;

		Spr_SetFrame(def, s->frame);
		Spr_Draw(def, bx, by);
	}
	(*CLASS(Tbl_FarAcolyte_vtable)->parent->method[5])(self);	/* ACTOR Draw */
}
