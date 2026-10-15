/* BigAcolyte_StateBurstWall - BIGACOLYTE state: the picture's last growth step bursts the wall.
 *
 * self: the boss.
 * Grows like BigAcolyte_ActGrow once more; throws 3 debris pieces from each of the 3x4 wall
 * blocks (columns $D7-$D9, rows $1B-$1E); rewrites those 12 map cells with broken-wall blocks
 * copied from map row $20, columns $11-$18 (some flipped); stops sound $DC and plays $34;
 * renders the picture; then it turns back into its sprite (BigAcolyte_StateBecomeSprite).
 * Returns self->flags & 8. */
#include "xmen2.h"

/* The block (without flips and hook bits) of FG map cell (x, y). */
#define FG_BLOCK(x, y)	(g_FgBlockMap[g_FgRowTable[y] + (x)] & 0x1FF)

short BigAcolyte_StateBurstWall(register BigAcolyte *self)
{
	register short i;
	register short x, y;
	register short j;

	self->x -= 0x100;
	self->y -= 0x180;
	g_ScaleParamsPtr->outW += 2;
	g_ScaleParamsPtr->outH += 3;
	for (i = 2; i >= 0; i--)
		for (j = 3; j >= 0; j--) {
			x = ((i + 0xD7) << 5) + 8;
			y = ((j + 0x1B) << 5) - 8;
			Debris_Spawn(x, y, g_BigAcolyteDebris, 3);
		}
	Map_SetBlockFGFlip(0xD7, 0x1B, FG_BLOCK(0x11, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD7, 0x1C, FG_BLOCK(0x15, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD7, 0x1D, FG_BLOCK(0x16, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD7, 0x1E, FG_BLOCK(0x12, 0x20), 0, 1, 1);
	Map_SetBlockFGFlip(0xD8, 0x1B, FG_BLOCK(0x13, 0x20), 0, 1, 0);
	Map_SetBlockFGFlip(0xD8, 0x1C, FG_BLOCK(0x18, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD8, 0x1D, FG_BLOCK(0x18, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD8, 0x1E, FG_BLOCK(0x14, 0x20), 0, 0, 1);
	Map_SetBlockFGFlip(0xD9, 0x1B, FG_BLOCK(0x12, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD9, 0x1C, FG_BLOCK(0x17, 0x20), 0, 0, 0);
	Map_SetBlockFGFlip(0xD9, 0x1D, FG_BLOCK(0x15, 0x20), 0, 1, 1);
	Map_SetBlockFGFlip(0xD9, 0x1E, FG_BLOCK(0x11, 0x20), 0, 1, 1);
	Snd_Stop(0xDC);
	Snd_Stop(0x34);
	Snd_Play(0x34);
	BigAcolyte_RenderScaled((ScalePicture *)g_ScaleSrcBitmap, g_ScaleParamsPtr,
		self->spr.tileBase << 5, 0);
	SET_STATE(self, BigAcolyte_StateBecomeSprite);
	return self->flags & 8;
}
