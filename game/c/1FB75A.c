/* Spr_EmitPre - put a preloaded (type $20) sprite frame that lies wholly on screen into the
 * hardware sprite list (Spr_Draw).
 *
 * spr: the instance (its uploaded frame; nothing when that is negative).  x, y: its position in
 * sprite space (+$80; the slot's full long, see below).  tile: its tileBase.
 * At most 80 - g_SprIndex pieces (compared as bytes, as the original). Each 6-byte piece fills
 * the next consecutive slot after g_SprWritePtr (its link byte is kept): y, size, attr = the
 * piece's tile with the base's palette/priority and the instance's flips toggled by the
 * piece's, x; flipped instances mirror the offsets with Tbl_SprWidthPre/HeightPre. The piece y is
 * a byte read as -$40..$BF.
 * Hand-written 68000 in an MPW frame (the inner piece code is Spr_EmitPre_Piece). Two of its
 * quirks: the attribute base takes bits 0-12 from bits 19-31 of the x argument (a register it
 * didn't clear; 0 for on-screen x), and it stores that base into its own tile argument slot,
 * which the C can't do. */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

void Spr_EmitPre(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiecePre *p;
	HwSprite *hw;
	short count, n, base, a, size, sx, sy;
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
	hflip = vflip = 0;
	base = tile | (short)((unsigned long)x >> 19 & 0x1FFF) | ((u8)flags & 7) << 13;
	if (flags & 0x10) {
		hflip = 1;
		base |= 0x800;
	}
	if (flags & 8) {
		vflip = 1;
		base |= 0x1000;
	}
	p = (SprPiecePre *)((u8 *)def + (u16)f->pieces);
#if defined(XMEN2_NATIVE)
	for (n = 0; n < count; n++, p++) {	/* Spr_EmitPre_Piece */
#else
	for (n = 0; (s8)n < (s8)count; n++, p++) {	/* Spr_EmitPre_Piece */
#endif
		a = p->tile | ((base & 0xF800) ^ (p->attr & 0x1800));
		size = p->attr & 0xF;
		sx = p->x;
		sy = (s8)(p->y + 0x40) - 0x40;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthPre)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightPre)[size];
		sy += (short)y;
		hw++;
		hw->y = sy;
		hw->size = (u8)size;
		hw->attr = a;
		hw->x = sx;
	}
	g_SprWritePtr = hw;
	g_SprIndex += n;
}
