/* Hologram_Draw - HOLOGRAM Draw (class slot +$1A, replaces the parent's): the hologram as six
 * hardware sprites of 3 x 4 tiles (24 x 32 px), two columns of three, straight into the sprite
 * list, then a terminator sprite.
 *
 * self: the hologram.
 * Nothing is drawn in pose $1B (hidden). Its top left corner is 24 px left of and 96 px above
 * its position; when that is off screen (x outside -32..352, y outside 0..200) nothing is
 * drawn and the flicker is armed with no sprite slot. Tiles: from its sprite's VRAM base, 12
 * per pose; the left column's three rows start at tile 0, $54 and $A8 (7 poses each); the
 * right column mirrors the left one (h-flip) in poses 0-4 and has its own tiles in poses 5-6
 * (rows at $FC, $114, $12C). The flicker (Hologram_VBlankFlicker) gets the slot before the
 * first sprite and the slot after the last one, and is armed. The terminator sprite is at
 * screen (-100, -100) (y and x $1C), size 1 x 1, tile 0.
 * The original multiplied the pose by 12 with shifts through a temporary, a different one at
 * each use (t0-t5, kept on the stack); only tile was declared register (it gets d7, the
 * others go by use count). */
#include "xmen2.h"

#define g_HologramPose	(*(s32 *)g_SceneVar3B14)	/* scene variable: pose drawn (Hologram_SetPose) */

/* p * 12, through the temporary t */
#define TIMES12(t, p)	((t) = (p), ((t) << 3) + ((t) << 2))

/* The next hardware sprite of the list: 3 x 4 tiles (size $B) at screen (sx, sy), attributes a. */
#define HOLO_SPRITE(sx, sy, a) \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = (sy) + 0x80; \
	g_SprWritePtr->size = 0xB; \
	g_SprWritePtr->attr = (a); \
	g_SprWritePtr->x = (sx) + 0x80

void Hologram_Draw(register Hologram *self)
{
	short sx, sy;
	short off;
	register short tile;
	long t0, t1, t2;

	if (g_HologramPose == 0x1B)
		return;
	g_HoloFlickerLinks[0] = g_SprWritePtr->link - 1;
	tile = self->spr.tileBase;
	sx = (self->x >> 8) - 24 - g_CameraX;
	sy = (self->y >> 8) - 96 - g_CameraY;
	if (sx < -32 || sx > 352 || sy < 0 || sy > 200) {
		g_HoloFlickerLinks[1] = 0;
		g_HoloFlickerArmed = 1;
		return;
	}
	off = (short)TIMES12(t0, g_HologramPose);
	HOLO_SPRITE(sx, sy, tile + off);
	off = (short)(TIMES12(t1, g_HologramPose) + 0x54);
	HOLO_SPRITE(sx, sy + 0x20, tile + off);
	off = (short)(TIMES12(t2, g_HologramPose) + 0xA8);
	HOLO_SPRITE(sx, sy + 0x40, tile + off);
	if (g_HologramPose <= 4) {
		long t3, t4, t5;

		off = (short)TIMES12(t3, g_HologramPose);
		HOLO_SPRITE(sx + 0x18, sy, (short)(tile + off) | 0x800);
		off = (short)(TIMES12(t4, g_HologramPose) + 0x54);
		HOLO_SPRITE(sx + 0x18, sy + 0x20, (short)(tile + off) | 0x800);
		off = (short)(TIMES12(t5, g_HologramPose) + 0xA8);
		HOLO_SPRITE(sx + 0x18, sy + 0x40, (short)(tile + off) | 0x800);
	} else {
		long t3, t4, t5;

		off = (short)(TIMES12(t3, g_HologramPose - 5) + 0xFC);
		HOLO_SPRITE(sx + 0x18, sy, tile + off);
		off = (short)(TIMES12(t4, g_HologramPose - 5) + 0x114);
		HOLO_SPRITE(sx + 0x18, sy + 0x20, tile + off);
		off = (short)(TIMES12(t5, g_HologramPose - 5) + 0x12C);
		HOLO_SPRITE(sx + 0x18, sy + 0x40, tile + off);
	}
	g_HoloFlickerLinks[1] = g_SprWritePtr->link;
	g_HoloFlickerArmed = 1;
	(g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link))->y = 0x1C;
	g_SprWritePtr->size = 0;
	g_SprWritePtr->attr = 0;
	g_SprWritePtr->x = 0x1C;
}
