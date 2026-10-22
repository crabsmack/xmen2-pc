/* Spr_EmitPreClip - Spr_EmitPre for a preloaded frame that crosses the screen edge (Spr_Draw).
 *
 * spr: the instance.  x, y: its position in sprite space (+$80).  tile: its tileBase.
 * Nothing when the list is full (g_SprIndex 80) or no frame is uploaded. As Spr_EmitPre
 * (Tbl_SprWidthPreClip/HeightPreClip), but a piece outside $60..$1C0 x $60..$160 is left out,
 * and the list ends after the slot linked to 80 (the sentinel). g_SprIndex grows by the slots
 * written. Hand-written 68000 in an MPW frame (the inner piece code is Spr_EmitPreClip_Piece);
 * it stores its attribute base into its own tile argument slot, which the C can't do. */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

void Spr_EmitPreClip(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiecePre *p;
	HwSprite *hw;
	s8 count;
	short n, base, a, size, sx, sy;
	char hflip, vflip;

#if defined(XMEN2_NATIVE)
	plat_spr_anchor(spr, x, y);	/* phase 6 R4: the pieces written next are spr's, at its anchor */
#endif
	if (g_SprIndex >= 80)
		return;
	if ((s8)spr->uploaded < 0)
		return;
	hw = g_SprWritePtr;
	f = &def->frame[spr->uploaded];
	count = f->pieceCount;
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
	p = (SprPiecePre *)((u8 *)def + (u16)f->pieces);
	for (n = 0; (s8)n < count; n++, p++) {
		a = p->tile | ((base & 0xF800) ^ (p->attr & 0x1800));
		size = p->attr & 0xF;
		sx = p->x;
		sy = (s8)(p->y + 0x40) - 0x40;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthPreClip)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightPreClip)[size];
		sy += (short)y;
		/* Spr_EmitPreClip_Piece */
		if (sx < VIEW_L(0x60) || sx > VIEW_R(0x1C0) || sy < 0x60 || sy > 0x160)
			continue;
		hw++;
		hw->y = sy;
		hw->size = (u8)size;
		hw->attr = a;
		hw->x = sx;
		if (hw->link == 0x50)
			break;
	}
	g_SprIndex += (short)(hw - g_SprWritePtr);
	g_SprWritePtr = hw;
}
