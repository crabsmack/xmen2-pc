/* CoreShields_Draw - CORESHIELDS Draw (class slot +$1A; the parent's isn't called): the ring of
 * panels as hardware sprites written straight into the sprite table, from the layout of its
 * phase (Tbl_CoreShieldsLayout0/1: 5 panels, Layout2: 4): first the front panels, high
 * priority, counting back from the front one (rotation, rotation - 1, ... wrapping 0 to 7), each
 * unless broken; then the back panels (tiles + $38, low priority), counting forward from the
 * front one (from the next one in phase 2), each unless broken, and in the 5-panel phases: never
 * the middle one, the last only in phase 1, the first only in phase 0; in phase 2 only where the
 * front panel counted back to the same step is broken (a hole shows the back).
 *
 * self: the shields.
 * While the hit flash runs, palette 3 every other 4 frames (attribute bits $6000). The pieces
 * aren't tested against the screen. The ROM's frame has 8 bytes no instruction uses: two dead
 * locals whose address an if (0) takes (MPW C keeps them on the stack). */
#include "xmen2.h"

/* the next hardware sprite: piece r at screen (sx, sy), attribute base a */
#define EMIT_PIECE(r, sx, sy, a) \
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link); \
	g_SprWritePtr->y = (sy) + (r)->dy + 0x80; \
	g_SprWritePtr->size = (r)->size & 0xF; \
	g_SprWritePtr->attr = (short)((a) + (r)->tile) | (r)->attrHi << 8; \
	g_SprWritePtr->x = (sx) + (r)->dx + 0x80

void CoreShields_Draw(register CoreShields *self)
{
	char back = self->rotation;
	short sx = (self->x >> 8) - g_CameraX;
	short sy = (self->y >> 8) - g_CameraY;
	short count = 0;
	long unused1;	/* two dead stack locals: the ROM's frame has their 8 bytes */
	long unused2;
	char front;
	register short attr;
	register short i;
	register ShieldPiece *r;

	if (!self->phase) {
		r = (ShieldPiece *)CODEPTR(Tbl_CoreShieldsLayout0);
		count = 5;
		front = self->rotation;
	} else if (self->phase == 1) {
		r = (ShieldPiece *)CODEPTR(Tbl_CoreShieldsLayout1);
		count = 5;
		front = self->rotation;
	} else {
		r = (ShieldPiece *)CODEPTR(Tbl_CoreShieldsLayout2);
		count = 4;
		front = self->rotation + 1;
	}
	attr = self->spr.tileBase;
	if (self->hitCooldown > 0 && (g_FrameCounter & 4))
		attr |= 0x6000;
	attr |= 0x8000;
	for (i = 0; i < count; i++, r++, back--) {
		if (back < 0)
			back = 7;
		if (!self->broken[back]) {
			EMIT_PIECE(r, sx, sy, attr);
		}
	}
	attr &= 0x7FFF;
	attr += 0x38;
	back = self->rotation;
	for (i = 0; i < count; i++, r++, front++, back--) {
		if (front >= 8)
			front = 0;
		if (self->broken[front])
			continue;
		if (back < 0)
			back = 7;
		if (count == 5) {
			if (i == 2)
				continue;
			if (i == 4) {
				if (!self->phase)
					continue;
			} else if (i == 0) {
				if (self->phase == 1)
					continue;
			}
		} else if (!self->broken[back])
			continue;
		EMIT_PIECE(r, sx, sy, attr);
	}
	if (0) {
		(void)&unused1;
		(void)&unused2;
	}
}
