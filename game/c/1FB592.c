/* Spr_EmitPreLinked - Spr_EmitPre for a preloaded sprite that carries objects (Spr_Draw).
 *
 * spr: the instance.  x, y: its position in sprite space (+$80).  tile: its tileBase.
 * Every piece is clipped to $60..$1C0 x $60..$160; a piece with tile 0 is the anchor of the
 * attached objects instead: Spr_PlaceAttached at its offset (x mirrored by the h-flip only;
 * when there are any). A piece with attr bit 15 is also left out while the first byte of the
 * instance's partStates is set. A piece left out makes the result -1 ($FF). Each piece written goes
 * into the slot the write pointer links to (g_SprIndex, g_SprWritePtr move there; its link =
 * its index + 1); the list being full there (link 80) ends it all with -1. Returns 0 when no
 * frame is uploaded or nothing was left out. Hand-written 68000 in an MPW frame (the inner
 * piece code is Spr_EmitPreLinked_Piece); it stores its attribute base into its own tile
 * argument slot, which the C can't do, and leaves Spr_PlaceAttached's arguments on its stack. */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

char Spr_EmitPreLinked(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiecePre *p;
	HwSprite *hw;
	short n, base, a, size, sx, sy, i;
	char hflip, vflip, clipped;

	clipped = 0;
	if ((s8)spr->uploaded < 0)
		return clipped;
	hflip = vflip = 0;
	base = tile | ((u8)flags & 7) << 13;
	if (flags & 0x10) {
		hflip = 1;
		base |= 0x800;
	}
	if (flags & 8) {
		vflip = 1;
		base |= 0x1000;
	}
	f = &def->frame[spr->uploaded];
	p = (SprPiecePre *)((u8 *)def + (u16)f->pieces);
	for (n = 0; (s8)n < f->pieceCount; n++, p++) {
		if (p->tile == 0) {
			if (spr->firstChild != 0xFF) {
				sy = (short)((s8)(p->y + 0x40) - 0x40) + (short)y;
				sx = (spr->flags & 0x10 ? -p->x : p->x) + (short)x;
				Spr_PlaceAttached(spr, sx, sy);
			}
			continue;
		}
		a = p->tile | ((base & 0xF800) ^ (p->attr & 0x1800));
		size = p->attr & 0xF;
		sx = p->x;
		sy = (s8)(p->y + 0x40) - 0x40;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthLinked)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightLinked)[size];
		sy += (short)y;
		/* Spr_EmitPreLinked_Piece */
		if (sx < 0x60 || sx > 0x1C0 || sy < 0x60 || sy > 0x160
		    || (p->attr < 0 && *(s8 *)&spr->partStates != 0)) {
			clipped = -1;
			continue;
		}
		i = (u8)g_SprWritePtr->link;
		if (i == 0x50)
			return -1;
		g_SprIndex = i;
		hw = g_SpriteTable + i;
		g_SprWritePtr = hw;
		hw->y = sy;
		hw->size = (u8)size;
		hw->link = (s8)(i + 1);
		hw->attr = a;
		hw->x = sx;
	}
	return clipped;
}
