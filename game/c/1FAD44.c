/* Spr_UploadFrame - send the tiles of a sprite instance's frame to its VRAM tiles.
 *
 * spr: a streamed sprite instance.  now: 0 queue the DMA (VDP_QueueDma), else write the VDP now.
 * The frame becomes the uploaded one. Nothing more when it is already up to date (flags bit 5),
 * when it shows no frame, or for a preloaded definition (type $20); else bit 5 is set and the
 * frame's pieces go to consecutive tiles from tileBase: raw tiles (type $18: 8-byte pieces, the
 * size byte's high nibble + 1 tiles), or tile blocks (4-byte pieces; a block with any of size
 * bits $9CC0 stands for another, Tbl_SprSizeTiles[size] tiles): raw words for type $14 or when
 * queued, else Nemesis-packed (Nem_Decode straight to VRAM, with a code table never built:
 * whatever the stack held). Writing now is done with interrupts off per piece. */
#include "xmen2.h"


/* The VDP control word for a VRAM write at byte address a, as the original summed it. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))
#define PIECE8(p)	((SprPiece8 *)(p))
#define PIECE4(p)	((SprPiece4 *)(p))

void Spr_UploadFrame(SprInst *spr, char now)
{
	SprDef *def;
	SprFrame *f;
	NemCodeTable tab;
	short tile;
	short i;
	register long off;
	register short n;
	register u8 *p;
	register SprTiles *src;

	def = (SprDef *)spr->def;
#ifndef XMEN2_MPW
	if (!def)	/* no definition: the 68000 reads the vector table */
		def = (SprDef *)Vectors;
#endif
	spr->uploaded = spr->frame;
	if (spr->flags & 0x20)
		return;
	if (spr->frame == 0xFF)
		return;
	if (def->type == 0x20)
		return;
	spr->flags |= 0x20;
	tile = spr->frame;
	f = SPR_FRAME(def, tile);
	tile = spr->tileBase;
	i = f->tileCount;
	p = (u8 *)def + f->pieces;
	if (now) {
		if ((s8)def->type == 0x18) {
			for (i = 0; i < f->pieceCount; p += sizeof(SprPiece8), i++) {
				off = PIECE8(p)->tiles;
				if (off > 0) {
					src = (SprTiles *)((u8 *)def + off);
					n = ((u8)PIECE8(p)->attr >> 4) + 1;
					Irq_Disable();
					VDP_CTRL32 = VRAM_WRITE(tile << 5);
					VDP_WriteWords(src, n << 4);
					Irq_Enable();
					tile += n;
				}
			}
		} else {
			for (i = 0; i < f->pieceCount; p += sizeof(SprPiece4), i++) {
				off = PIECE4(p)->tiles;
				if (off > 0) {
					src = (SprTiles *)((u8 *)def + off);
					if ((src->size & 0x9CC0) != 0)
						src = (SprTiles *)((u8 *)def + src->ref);
					n = *((s8 *)CODEPTR(Tbl_SprSizeTiles) + src->size);
					Irq_Disable();
					if ((s8)def->type == 0x14) {
						VDP_CTRL32 = VRAM_WRITE(tile << 5);
						VDP_WriteWords((s16 *)src + 1, n << 4);
					} else
						Nem_Decode((s16 *)src + 1, &tab, n << 3, 1, tile << 5);
					Irq_Enable();
					tile += n;
				}
			}
		}
	} else {
		if ((s8)def->type == 0x18) {
			for (i = 0; i < f->pieceCount; p += sizeof(SprPiece8), i++) {
				off = PIECE8(p)->tiles;
				if (off > 0) {
					src = (SprTiles *)((u8 *)def + off);
					n = ((u8)PIECE8(p)->attr >> 4) + 1;
					VDP_QueueDma(src, tile << 5, n << 5);
					tile += n;
				}
			}
		} else {
			for (i = 0; i < f->pieceCount; p += sizeof(SprPiece4), i++) {
				off = PIECE4(p)->tiles;
				if (off > 0) {
					src = (SprTiles *)((u8 *)def + off);
					if ((src->size & 0x9CC0) != 0)
						src = (SprTiles *)((u8 *)def + src->ref);
					n = *((s8 *)CODEPTR(Tbl_SprSizeTiles) + src->size);
					VDP_QueueDma((s16 *)src + 1, tile << 5, n << 5);
					tile += n;
				}
			}
		}
	}
}

