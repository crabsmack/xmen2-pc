/* Cerebro_BigSpriteInit - set a big sprite up to show a picture at its own size: its scaler
 * parameters get the picture's box (width rounded up to even) as source, rectangle and output
 * size, the tile grid that covers it ((w + 7) / 8 x (h + 7) / 8), clear the buffer, rebuild the
 * step tables, no flips, no transpose, scaled into tiles, not packed, all rows in one call.
 *
 * bs: the big sprite.  pic: the picture (a bitmap: box at +2..+5, pixels from +$26).
 * Its sprite flags become 0. */
#include "xmen2.h"

void Cerebro_BigSpriteInit(BigSprite *bs, register ScalePicture *pic)
{
	register ScaleParams *p = bs->params;

	p->srcW = pic->right - pic->left;
	p->srcW += p->srcW & 1;
	p->srcH = pic->bottom - pic->top;
	p->srcX = 0;
	p->srcY = 0;
	p->rectW = p->srcW;
	p->rectH = p->srcH;
	p->tilesW = (p->srcW + 7) >> 3;
	p->tilesH = (p->srcH + 7) >> 3;
	p->outW = p->srcW;
	p->outH = p->srcH;
	p->clearMode = 1;
	p->tableKey = 1;
	p->unk_16 = 0;
	p->unk_18 = 0;
	p->direct = 1;
	p->packed = 0;
	p->rowsPerCall = 0;
	p->unk_1E = 0;
	p->unk_20 = 0;
	p->unk_22 = 0;
	p->unk_24 = 0;
	bs->pic = pic;
	bs->sprFlags = 0;
}
