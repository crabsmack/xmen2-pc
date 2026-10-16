/* Spr_PreloadTiles - turn a streamed sprite definition into a preloaded one: a RAM copy whose
 * tiles are all in VRAM.
 *
 * spr: the sprite instance (its definition, raw-tile type $18 or Nemesis-packed $19).
 * tile: the first VRAM tile to use.
 * At the heap top (g_HeapTop): the $24-byte header (type $20) and the frame records, then each
 * frame's pieces as 6-byte preloaded pieces (x, y bytes; size and flips | $4000 while the tile is
 * still the ROM offset of the piece's tiles) and its hit-point list (words up to 0), the records'
 * offsets rewritten for the copy. Then each distinct tile block is uploaded once
 * (Tbl_SprSizeTiles[size] tiles; Nem_Decode for $19 with the definition's code table, else
 * VDP_WriteWords) and every piece using it gets its VRAM tile. The instance then points at the
 * copy with that tile range; the heap top moves past it (even). Returns the next free tile. */
#include "xmen2.h"

/* The VDP control word for a VRAM write at byte address a, as the original summed it. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))
#define PIECE8(p)	((SprPiece8 *)(p))
#define PIECE(p)	((SprPiecePre *)(p))
#define FRAME(p)	((SprFrame *)(p))
#define HEADER		0x24	/* bytes of a sprite definition before its frame records (SprDef.frame) */

short Spr_PreloadTiles(SprInst *spr, short tile)
{
	SprDef *def;
	u8 *copy;
	u8 *src;
	u8 *dst;
	u8 *rd;
	u8 *first;
	u8 *end;
	u8 *data;
	u8 *q;
	NemCodeTable tab;
	short unused;
	register short i;
	register short n;
	register short next;
	register short size;
	register u8 *p = (u8 *)g_HeapTop;
	register u8 *out;

	def = (SprDef *)spr->def;
	copy = p;
	next = tile;
	src = (u8 *)def;
	dst = p;
	for (i = 0; i < HEADER; i++)
		*dst++ = *src++;
	*copy = 0x20;
	if (def->type == 0x19)
		p = Nem_BuildCodeTable((u8 *)def + def->codes, &tab);
	out = copy;
	out += (s16)FRAME(src)->pieces;
	first = out;
	for (n = 0; n < (s8)def->frameCount; n++) {
		q = src;
		p = dst;
		for (i = 0; i < 0x14; i++)
			*p++ = *q++;
		rd = (u8 *)def + (s16)FRAME(src)->pieces;
		FRAME(dst)->pieces = (short)ADDR68(out) - (short)ADDR68(copy);
		unused = 0;
		for (size = 0; size < FRAME(src)->pieceCount; size++) {
			*(s16 *)out = (PIECE8(rd)->x & 0xFF) << 8 | PIECE8(rd)->y & 0xFF;
			PIECE(out)->attr = 0;
			PIECE(out)->tile = 0;
			if ((i = PIECE8(rd)->tiles) > 0) {
				PIECE(out)->attr = PIECE8(rd)->attr & 0x180F | 0x4000;
				PIECE(out)->tile = i;
			}
			out += sizeof(SprPiecePre);
			rd += sizeof(SprPiece8);
		}
		src += sizeof(SprFrame);
		dst += sizeof(SprFrame);
	}
	end = out;
	src = (u8 *)def + HEADER;
	dst = copy + HEADER;
	for (n = 0; n < (s8)def->frameCount; n++) {
		if (i = FRAME(src)->hitPoints) {
			rd = (u8 *)def + i;
			FRAME(dst)->hitPoints = (short)ADDR68(out) - (short)ADDR68(copy);
			do {
				i = *(s16 *)rd;
				rd += 2;
				*(s16 *)out = i;
				out += 2;
			} while (i);
		}
		src += sizeof(SprFrame);
		dst += sizeof(SprFrame);
	}
	for (q = first; q < end; q += sizeof(SprPiecePre)) {
		if (PIECE(q)->attr & 0x4000) {
			i = PIECE(q)->tile;
			data = (u8 *)def + i;
			size = (s8)PIECE(q)->attr;
			for (p = q; p < end; p += sizeof(SprPiecePre)) {
				if (i == PIECE(p)->tile && (PIECE(p)->attr & 0x4000)) {
					PIECE(p)->attr &= ~0x4000;
					PIECE(p)->tile = next;
				}
			}
			n = *((s8 *)CODEPTR(Tbl_SprSizeTiles) + size);
			if (def->type == 0x19)
				Nem_Decode(data, &tab, n << 3, 1, next << 5);
			else {
				VDP_CTRL32 = VRAM_WRITE(next << 5);
				VDP_WriteWords(data, n << 4);
			}
			next += n;
		}
	}
	spr->def = copy;
	g_HeapTop = out;
	if (ADDR68(out) & 1)
		(*(s32 *)&g_HeapTop)++;
	spr->tileCount = next - tile;
	spr->tileBase = tile;
	return next;
}
