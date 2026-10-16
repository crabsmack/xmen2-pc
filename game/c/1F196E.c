/* Spr_UploadFrame18 - queue the DMA of a raw-tile (type $18) sprite instance's frame tiles.
 *
 * spr: the instance (XPlayer_Draw, when the frame isn't up to date).
 * Like Spr_UploadFrame's queued type $18 path, without its checks: the frame becomes the uploaded
 * one (nothing more when it is $FF), flags bit 5 is set, and each piece's raw tiles (the size
 * byte's high nibble + 1 tiles) are queued to consecutive tiles from tileBase. */
#include "xmen2.h"

void Spr_UploadFrame18(SprInst *spr)
{
	SprDef *def;
	short count;
	register short tile;
	register short n;
	register short i;
	register long off;
	register SprFrame *f;
	u8 *p;
	u8 *src;

	tile = spr->uploaded;
	spr->uploaded = spr->frame;
	if (spr->frame == 0xFF)
		return;
	def = (SprDef *)spr->def;
	spr->flags |= 0x20;
	tile = spr->frame;
	f = SPR_FRAME(def, tile);
	tile = spr->tileBase;
	p = (u8 *)def + f->pieces;
	count = f->pieceCount;
	for (i = 0; i < count; p += sizeof(SprPiece8), i++) {
		off = ((SprPiece8 *)p)->tiles;
		if (off > 0) {
			src = (u8 *)def + off;
			n = ((u8)((SprPiece8 *)p)->attr >> 4) + 1;
			VDP_QueueDma(src, tile << 5, n << 5);
			tile += n;
		}
	}
}
