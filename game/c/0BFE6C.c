/* SwampPlatform_Draw - SWAMPPLATFORM Draw (class slot +$1A; the parent's isn't called): two 32x32
 * hardware sprites side by side (tiles tile and tile + $10), straight into the sprite table, 32 px
 * above its position: the left one ending at x, the right one starting there.
 *
 * self: the platform.
 * Each sprite is drawn only with its left x and top y inside -32 .. $140 / $E0 (screen pixels,
 * exclusive).
 * MPW: each sprite has its offsets written out, even the zero ones (tile + 0, sx - 0): the ROM
 * copies those values through d0, and sx - 0 + $80 folds to its sub.w #-$80. */
#include "xmen2.h"

/* One 32x32 hardware sprite at screen (x, y) when on screen. */
#define EMIT_32X32(X, Y, ATTR) \
	if ((X) > -0x20 && (X) < 0x140 && (Y) > -0x20 && (Y) < 0xE0) { \
		g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
		g_SprWritePtr->y = (Y) + 0x80; \
		g_SprWritePtr->size = 0xF; \
		g_SprWritePtr->attr = (ATTR); \
		g_SprWritePtr->x = (X) + 0x80; \
	}

void SwampPlatform_Draw(register SwampPlatform *self)
{
	register long tile = self->tile;
	register long sx = (self->x >> 8) - g_CameraX;
	register long sy = (self->y >> 8) - g_CameraY - 0x20;

	EMIT_32X32(sx - 0x20, sy, tile + 0);
	EMIT_32X32(sx - 0, sy, tile + 0x10);
}
