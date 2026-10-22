/* Spr_EmitStream - put a raw-tile (type $18/$19) sprite frame that lies wholly on screen into the
 * hardware sprite list (Spr_Draw, XPlayer_Draw).
 *
 * spr: the instance (its uploaded frame; nothing when that is negative).  x, y: its position in
 * sprite space (+$80).  tile: its first VRAM tile.
 * At most 80 - g_SprIndex pieces (compared as bytes, as the original). Each 8-byte piece fills
 * the next consecutive slot after g_SprWritePtr (its link byte is kept): y, size, attr = the
 * running tile number (11 bits) with the palette/priority and the instance's flips toggled by
 * the piece's, x; flipped instances mirror the offsets with Tbl_SprWidthStream/HeightStream.
 * The tile number moves on by Tbl_SprTilesStream[size] after each piece. Hand-written 68000 in
 * an MPW frame (the inner piece code is Spr_EmitStream_Piece). */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

void Spr_EmitStream(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiece8 *p;
	HwSprite *hw;
	short count, n, base, t, a, size, sx, sy;
	char hflip, vflip;

#if defined(XMEN2_NATIVE)
	plat_spr_anchor(spr, x, y);	/* phase 6 R4: the pieces written next are spr's, at its anchor */
#endif
	if ((s8)spr->uploaded < 0)
		return;
#if defined(XMEN2_NATIVE)
	/* phase 7: the raised stop (SPR_STOP) needs plain compares (the original's bytes hold to 127);
	   the pieces the stop leaves out are counted (the mp counters' sprites_dropped) */
	f = &def->frame[spr->uploaded];
	count = SPR_STOP - g_SprIndex;
	if (count <= 0) {
		plat_spr_dropped((u8)f->pieceCount);
		return;
	}
	if (count > (u8)f->pieceCount)
		count = (u8)f->pieceCount;
	else
		plat_spr_dropped((u8)f->pieceCount - count);
#else
	count = 80 - g_SprIndex;
	if (count <= 0)
		return;
	f = &def->frame[spr->uploaded];
	if ((s8)count > f->pieceCount)
		count = (u8)f->pieceCount;
#endif
	hw = g_SprWritePtr;
	n = 0;
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
	p = (SprPiece8 *)((u8 *)def + (s16)f->pieces);
#if defined(XMEN2_NATIVE)
	for (; n < count; n++, p++) {
#else
	for (; (s8)n < (s8)count; n++, p++) {
#endif
		t = (t & 0x7FF) | base;
		size = p->attr & 0xF;
		a = t ^ (p->attr & 0x1800);
		sx = p->x;
		sy = p->y;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthStream)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightStream)[size];
		sy += (short)y;
		/* Spr_EmitStream_Piece */
		hw++;
		hw->y = sy;
		hw->size = (u8)size;
		hw->attr = a;
		hw->x = sx;
		t = a + TBL(Tbl_SprTilesStream)[size];
	}
	g_SprWritePtr = hw;
	g_SprIndex += n;
}
