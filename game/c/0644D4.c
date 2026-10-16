/* Cerebro_BigSpriteDecode - scale a big sprite's picture into the scaler's tile buffer
 * (g_ScaleTileBuf), for its VRAM tiles from tileBase: Scaler_Start with its parameters, then one
 * Scaler_Step. A packed (streamed) pass uploads as it goes, so the sprite counts as uploaded; else
 * Cerebro_BigSpriteDraw uploads the buffer.
 *
 * bs: the big sprite. */
#include "xmen2.h"

void Cerebro_BigSpriteDecode(register BigSprite *bs)
{
	Scaler_Start(bs->params, 0, bs->pic->pixels, g_ScaleTileBuf, bs->tileBase << 5);
	Scaler_Step();
	if (bs->params->packed)
		bs->uploaded = 1;
	else
		bs->uploaded = 0;
}
