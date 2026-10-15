/* Spr_Init - set a sprite instance up for a sprite definition and reserve its VRAM tiles.
 *
 * spr: the instance.  unused: ignored (1 from every caller).  def: the sprite definition (0:
 * none).  frame: the frame shown ($FF none; -1: reserve for the largest frame).  pal: palette
 * (bits 0-1).  tile: its first VRAM tile.  upload: non-zero to upload the frame now.
 * Flags = the palette, nothing uploaded. A preloaded definition (type $20) takes no tiles. A
 * streamed one (flags bit 7) reserves the frame's tile count from tile on (frame -1: the
 * definition's maxTiles, or when 0 the largest frame's). Returns the next free VRAM tile. */
#include "xmen2.h"

short Spr_Init(SprInst *spr, long unused, void *def, short frame, short pal, short tile, char upload)
{
	short next = tile;
	unsigned short count;
	short i;
	short t;
	SprFrame *f;

	spr->flags = 0;
	spr->unk_0B = 0;
	spr->uploaded = 0xFF;
	spr->flags |= pal & 3;
	spr->frame = (u8)frame;
	if (spr->def = def) {
		if ((s8)((SprDef *)def)->type == 0x20)
			next = tile;
		else {
			if (frame != -1) {
				f = SPR_FRAME(def, frame);
				count = f->tileCount;
			} else if ((count = ((SprDef *)def)->maxTiles) == 0) {
				short n = (s8)((SprDef *)def)->frameCount;

				f = SPR_FRAME(def, 0);
				count = 0;
				for (i = 0; i < n; f += 1, i++) {
					t = f->tileCount;
					if (count < t)
						count = t;
				}
			}
			spr->tileBase = tile;
			spr->tileCount = count;
			spr->flags |= 0x80;
			next = tile + count;
		}
	} else {
		spr->tileBase = -1;
		next = tile;
	}
	if (upload)
		Spr_UploadFrame(spr, 1);
	return next;
}

