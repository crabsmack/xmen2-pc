/* Spr_EmitStreamClip - Spr_EmitStream for a raw-tile frame that crosses the screen edge
 * (Spr_Draw).
 *
 * spr: the instance.  x, y: its position in sprite space (+$80).  tile: its first VRAM tile.
 * Nothing when the list is full (g_SprIndex 80) or no frame is uploaded. As Spr_EmitStream
 * (Tbl_SprWidthStreamClip/HeightStreamClip/TilesStreamClip), but a piece outside $60..$1C0 x
 * $60..$160 is left out (its tiles still count), and the list ends after the slot linked to 80
 * (the sentinel). g_SprIndex grows by the slots written. Hand-written 68000 in an MPW frame (the
 * inner piece code is Spr_EmitStreamClip_Piece). */
#include "xmen2.h"

#define TBL(t)	((s16 *)CODEPTR(t))

void Spr_EmitStreamClip(SprInst *spr, long x, long y, short tile)
{
	SprDef *def = (SprDef *)spr->def;
	s8 flags = spr->flags;
	SprFrame *f;
	SprPiece8 *p;
	HwSprite *hw;
	short n, base, t, a, size, sx, sy;
	char hflip, vflip;

#if defined(XMEN2_NATIVE)
	plat_spr_anchor(spr, x, y);	/* phase 6 R4: the pieces written next are spr's, at its anchor */
#endif
	if (g_SprIndex >= 80)
		return;
	if ((s8)spr->uploaded < 0)
		return;
	hw = g_SprWritePtr;
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
		size = p->attr & 0xF;
		a = t ^ (p->attr & 0x1800);
		sx = p->x;
		sy = p->y;
		if (hflip)
			sx = -sx - TBL(Tbl_SprWidthStreamClip)[size];
		sx += (short)x;
		if (vflip)
			sy = -sy - TBL(Tbl_SprHeightStreamClip)[size];
		sy += (short)y;
		/* Spr_EmitStreamClip_Piece */
		if (sx >= VIEW_L(0x60) && sx <= VIEW_R(0x1C0) && sy >= 0x60 && sy <= 0x160) {
			hw++;
			hw->y = sy;
			hw->size = (u8)size;
			hw->attr = a;
			hw->x = sx;
			if (hw->link == 0x50)
				break;
		}
		t = a + TBL(Tbl_SprTilesStreamClip)[size];
	}
	g_SprIndex += (short)(hw - g_SprWritePtr);
	g_SprWritePtr = hw;
}
