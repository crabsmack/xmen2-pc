/* BroodClaw_Draw - BROODCLAW Draw (class slot +$1A): put the claw's four pieces straight into the
 * hardware sprite table, at the offsets of the first frame of its definition (Art_BroodClawSpr).
 *
 * self: the claw.
 * Only when its body box is within $40 px left / $180 px right of the camera, and on odd frames
 * of g_LevelFrame while it flashes (flags6 $40). Piece 0 is 8x16 (size 1) from the sprite's first
 * tile, pieces 1-3 32x32 (size $F) from tiles +2, +$12, +$22; all in the sprite's palette, high
 * priority with spr.flags bit 2. Each takes the next slot along the sprite table's link bytes
 * (g_SprWritePtr, g_SprIndex), as Spr_Emit* do. */
#include "xmen2.h"

/* the next hardware sprite: piece pc, size code sz */
#define EMIT_PIECE(sz) \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = (short)sy + pc->y + 0x80; \
	g_SprWritePtr->size = (sz); \
	g_SprWritePtr->attr = attr; \
	g_SprWritePtr->x = (short)sx + pc->x + 0x80

void BroodClaw_Draw(register BroodClaw *self)
{
	short sx;
	short sy;
	register short attr;
	register SprPiece8 *pc;
	SprFrame *fr;

	if (self->boxRight > g_CameraX - 0x40 && self->boxLeft < g_CameraX + 0x180
	    && (!(self->flags6 & 0x40) || (g_LevelFrame & 1) != 0)) {
		sx = (self->x >> 8) - g_CameraX;
		sy = (self->y >> 8) - g_CameraY;
		pc = (SprPiece8 *)CODEPTR(Art_BroodClawSpr);
		fr = SPR_FRAME(pc, 0);
		pc = (SprPiece8 *)(fr + 1);
		attr = self->spr.tileBase;
		attr |= (self->spr.flags & 3) << 13;
		if (self->spr.flags & 4)
			attr |= 0x8000;
		EMIT_PIECE(1);
		attr += 2;
		pc++;
		EMIT_PIECE(0xF);
		attr += 0x10;
		pc++;
		EMIT_PIECE(0xF);
		attr += 0x10;
		pc++;
		EMIT_PIECE(0xF);
		attr += 0x10;
		pc++;
	}
}
