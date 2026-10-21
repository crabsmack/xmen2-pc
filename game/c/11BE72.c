/* BioFlame_Draw - BIOFLAME Draw (class slot +$1A; replaces the parent's): its three flame sprites
 * straight into the hardware sprite table, each 4 px left of and 8 px above the nozzle plus its
 * flame offset, size 0 (8x8), the tile the sprite's frame (+1 beyond 72 px out, +2 beyond 87), its
 * palette and priority; each only when on screen.
 *
 * self: the flame.
 * A sprite further out sits 1 px lower ((tile step) >> 1). Each takes the next slot along the
 * sprite table's link bytes (g_SprWritePtr, g_SprIndex), as Spr_Emit* do. */
#include "xmen2.h"

#define DRAW_FLAME(f) \
	sx = base + (f); \
	if (ABS(f) > 0x57) \
		step = 2; \
	else if (ABS(f) > 0x48) \
		step = 1; \
	else \
		step = 0; \
	if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy + (step >> 1) > -32 && sy + (step >> 1) < 224) { \
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
		g_SprWritePtr->y = (short)(sy + (step >> 1)) + 0x80; \
		g_SprWritePtr->size = 0; \
		g_SprWritePtr->attr = attr + step; \
		g_SprWritePtr->x = sx + 0x80; \
	}

void BioFlame_Draw(register BioFlame *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;

	sx -= 4;
	sy -= 8;
	{
		short base = sx;
		short attr = (self->spr.tileBase + self->spr.frame) | (self->spr.flags & 3) << 13;
		short step;

		if (self->spr.flags & 4)
			attr |= 0x8000;
		DRAW_FLAME(self->flameX[0])
		DRAW_FLAME(self->flameX[1])
		DRAW_FLAME(self->flameX[2])
	}
}
