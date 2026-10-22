/* Spr_EmitFlicker - put an object's frame into the hardware sprite list, piece by piece, with
 * optional flicker and per-frame flips (BioNpc_DrawStunned, Blast_Draw).
 *
 * obj: the object (its sprite instance gives the frame and flips).  tiles: first tile of each
 * size code (bytes, added to the base tile).  base: the sprite instance whose tileBase the
 * tiles start at.  flicker: non-zero to skip pieces: piece i (counting down) is skipped while
 * (i + g_VBlankCount) & mask.  pal: palette.  hmask, vmask: the piece is flipped while
 * g_VBlankCount & hmask (vmask).
 * At most 80 - g_SprIndex pieces; nothing without a frame or definition. Each piece takes the
 * slot the write pointer links to, at the object's screen position (as Actor_Draw computes it)
 * + the piece offset (mirrored with Tbl_SprSizeWidth/Height when the object is flipped), with
 * priority, the palette and base->tileBase + tiles[size]. */
#include "xmen2.h"

void Spr_EmitFlicker(Actor *obj, u8 *tiles, SprInst *base, char flicker, unsigned char mask,
	unsigned char pal, unsigned char hmask, unsigned char vmask)
{
	long x;
	SprDef *def;
	SprFrame *f;
	short count;
	register Actor *o = obj;
	register SprInst *spr = &o->spr;
	register SprPiece8 *p;
	register short frame;
	short off;
	register long i;
	register long size;
	register long attr;
	register long y;

	frame = spr->frame;
	if (frame == 0xFF)
		return;
	if (!spr->def)
		return;
	f = SPR_FRAME(def = (SprDef *)spr->def, frame);
	off = f->pieces;
	{
#if defined(XMEN2_NATIVE)
		short avail = SPR_STOP - g_SprIndex;	/* phase 7: the raised stop; what it leaves out counted */

		count = f->pieceCount;
		if (avail < count) {
			plat_spr_dropped(count - (avail > 0 ? avail : 0));
			count = avail;
		}
#else
		short avail = 80 - g_SprIndex;

		count = f->pieceCount;
		if (avail < count)
			count = avail;
#endif
	}
	if (count <= 0)
		return;
	p = (SprPiece8 *)((u8 *)def + off);
	{
		short sx = o->anim && (((AnimScript *)o->anim)->flags & 0x30) == 0x20
			? ((o->x - o->animDist) >> 8) - g_CameraX : (o->x >> 8) - g_CameraX;
		short sy = o->anim && (((AnimScript *)o->anim)->flags & 0x30) == 0x30
			? ((o->y - o->animDist) >> 8) - g_CameraY : (o->y >> 8) - g_CameraY;
		short a = base->tileBase | 0x8000;
		u8 *widths = (u8 *)CODEPTR(Tbl_SprSizeWidth);
		u8 *heights = (u8 *)CODEPTR(Tbl_SprSizeHeight);

		for (i = count; i; p++, i--) {
			if (flicker && (g_VBlankCount + i & mask))
				continue;
			size = (char)p->attr & 0xF;
			attr = a + tiles[size];
			attr |= pal << 13;
			if (o->spr.flags & 0x10)
				x = sx - p->x - widths[size];
			else
				x = sx + p->x;
			if (o->spr.flags & 8)
				y = sy - p->y - heights[size];
			else
				y = sy + p->y;
			if (g_VBlankCount & hmask)
				attr |= 0x800;
			if (g_VBlankCount & vmask)
				attr |= 0x1000;
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = (short)y + 0x80;
			g_SprWritePtr->size = (u8)size;
			g_SprWritePtr->attr = (short)attr;
			g_SprWritePtr->x = (short)x + 0x80;
		}
	}
}
