/* Cerebro_BigSpriteDraw - show a big sprite: its tiles DMAed from the scaler's buffer
 * (g_ScaleTileBuf) to VRAM unless already uploaded, then its hardware sprites at its position
 * (Scaler_EmitSprites: whole pixels of x and y, pal, sprFlags).
 *
 * bs: the big sprite. */
#include "xmen2.h"

void Cerebro_BigSpriteDraw(register BigSprite *bs)
{
	if (!bs->uploaded) {
		VDP_QueueDma(g_ScaleTileBuf, bs->tileBase << 5, bs->tiles << 5);
		bs->uploaded = 1;
	}
	Scaler_EmitSprites(bs->x >> 16, bs->y >> 16, bs->pal, bs->sprFlags);
}
