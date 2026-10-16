/* LightBeam_Draw - LIGHTBEAM Draw (class slot +$1A; replaces ACTOR's): write the row's beam columns
 * straight into the hardware sprite table. Columns are spacing x 32 px apart from x = xOff plus
 * the plane-B H-scroll of the line above the top (the beams scroll with the background they hang
 * from); x past $180 wraps by $200, and a column is drawn only within -$10 (kind 0) / -8 .. $140.
 * Kind 0: three 16x32 sprites, tiles +$10, +8, +0 from the top down; kind 1: an 8x24 sprite
 * (tiles +$20) over an 8x32 (+$1C); kind 2: one 8x32 (+$18). Nothing when that line is off the
 * screen (0-$DF).
 *
 * self: the beam row.
 * Tiles count from the definition's first VRAM tile with bit $4000 (palette 2). Each sprite takes
 * the next slot along the sprite table's link bytes (g_SprWritePtr, g_SprIndex). */
#include "xmen2.h"

/* Put one hardware sprite at screen (sx, sy): the next slot of the sprite table. */
#define EMIT(sy, sz, at, sx) \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = (short)(sy) + 0x80; \
	g_SprWritePtr->size = (sz); \
	g_SprWritePtr->attr = (at); \
	g_SprWritePtr->x = (short)(sx) + 0x80

/* g_HScrollBuf as words: line i's plane A scroll at [2i], plane B at [2i + 1]. */
#define g_HScrollWord	((s16 *)g_HScrollBuf)

void LightBeam_Draw(LightBeam *self)
{
	short attr = g_SpriteDefs[self->sprDef].tileBase | 0x4000;	/* the definition's first tile, palette 2 */
	short attr18 = attr + 0x18;
	short attr1C = attr18 + 4;
	short step = self->spacing << 5;	/* x between columns, pixels */
	short left = self->count;		/* columns left */
	register long n = 0;			/* columns done (kind 1; never read) */
	register long sy = self->topY - g_CameraY;
	register long line = sy - 1;		/* the screen line above the top */
#ifdef XMEN2_MPW
	register long sx = self->xOff + g_HScrollWord[(line << 1) + 1];	/* its plane-B H-scroll */
#else
	/* PC: the 68000 reads the word even for a line off the screen (any RAM or ROM within 32 KB:
	   no side effect, and sx is not used then); the host must not read outside the buffer */
	register long sx = line < 0 || line > 0xDF ? 0 : self->xOff + g_HScrollWord[(line << 1) + 1];
#endif

	if (line < 0 || line > 0xDF)
		return;
	switch (self->kind) {
	case 0:
		while (left) {
			if (sx > 0x180)
				sx -= 0x200;
			if (sx >= -0x10 && sx <= 0x140) {
				EMIT(sy, 7, attr + 0x10, sx);
				EMIT(sy + 0x20, 7, attr + 8, sx);
				EMIT(sy + 0x40, 7, attr, sx);
			}
			left--;
			sx += step;
		}
		break;
	case 1:
		while (left) {
			if (sx > 0x180)
				sx -= 0x200;
			if (sx >= -8 && sx <= 0x140) {
				EMIT(sy, 2, attr1C + 4, sx);
				EMIT(sy + 0x18, 3, attr1C, sx);
			}
			left--;
			n++;
			sx += step;
		}
		break;
	case 2:
		while (left) {
			if (sx > 0x180)
				sx -= 0x200;
			if (sx >= -8 && sx <= 0x140) {
				EMIT(sy, 3, attr18, sx);
			}
			left--;
			sx += step;
		}
		break;
	}
}
