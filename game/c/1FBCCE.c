/* Spr_EmitStreamLinked - Spr_EmitStream for a raw-tile sprite that carries objects (Spr_Draw).
 *
 * spr: the instance.  x, y: its screen position (without the $80: added here).  tile: its first
 * VRAM tile.
 * A piece with no tiles (offset 0) is the anchor of the attached objects: Spr_PlaceAttached at
 * its offset (x mirrored by the h-flip only; when there are any). Every other piece is clipped
 * to $60..$1C0 x $60..$160 (a piece left out makes the result -1, $FF) and goes into the slot the
 * write pointer links to (g_SprIndex, g_SprWritePtr move there; that slot's link is kept); the
 * list being full there (link 80) ends it all with -1. The tile number moves on by
 * Tbl_SprTilesStreamLinked[size] after each piece but the anchors. Returns 0 when no frame is
 * uploaded or nothing was left out. Hand-written 68000 in an MPW frame (the inner piece code is
 * Spr_EmitStreamLinked_Piece); it leaves Spr_PlaceAttached's arguments on its stack. */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

char Spr_EmitStreamLinked(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiece8 *p;
	HwSprite *hw;
	short n, base, t, a, size, sx, sy, i;
	char hflip, vflip, clipped;

#if defined(XMEN2_NATIVE)
	plat_spr_anchor(spr, x + 0x80, y + 0x80);	/* phase 6 R4: the pieces written next are spr's, at its anchor */
#endif
	clipped = 0;
	if ((s8)spr->uploaded < 0)
		return clipped;
	t = tile;
	hflip = vflip = 0;
	base = ((u8)flags & 7) << 13;
	if (flags & 0x10) {
		hflip = 1;
		base |= 0x800;
	}
	if (flags & 8) {
		vflip = 1;
		base |= 0x1000;
	}
	f = &def->frame[spr->uploaded];
	p = (SprPiece8 *)((u8 *)def + (s16)f->pieces);
	for (n = 0; (s8)n < f->pieceCount; n++, p++) {
		t = (t & 0x7FF) | base;
		if (p->tiles == 0) {
			if (spr->firstChild != 0xFF)
				Spr_PlaceAttached(spr, (hflip ? -p->x : p->x) + (short)x, p->y + (short)y);
			continue;
		}
		size = p->attr & 0xF;
		a = t ^ (p->attr & 0x1800);
		sx = p->x;
		sy = p->y;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthStreamLinked)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightStreamLinked)[size];
		sy += (short)y;
		/* Spr_EmitStreamLinked_Piece */
		sx += 0x80;
		if (sx < VIEW_L(0x60) || sx > VIEW_R(0x1C0))
			clipped = -1;
		else if ((sy += 0x80) < 0x60 || sy > 0x160)
			clipped = -1;
		else {
			i = (u8)g_SprWritePtr->link;
			if (i == 0x50)
				return -1;
			g_SprIndex = i;
			hw = g_SpriteTable + i;
			g_SprWritePtr = hw;
			hw->y = sy;
			hw->size = (u8)size;
			hw->attr = a;
			hw->x = sx;
		}
		t = a + TBL(Tbl_SprTilesStreamLinked)[size];
	}
	return clipped;
}
