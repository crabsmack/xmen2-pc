/* BigAcolyte_RenderScaled - scale the big acolyte picture into the tile buffer and DMA it to VRAM.
 *
 * pic:   the picture (Art_BigAcolytePicture).
 * sp:    the scale record (output size outW x outH pixels, in a tilesW x tilesH tile grid).
 * vram:  VRAM address of the tiles.
 * first: first call: allocate the tile buffer g_ScaleTileBuf (tilesW << 7 bytes when packed,
 *        else tilesW * tilesH tiles) and start the scaler once more (Scaler_InitParams first).
 * Runs one scaler job (Scaler_Start, Scaler_Step) into g_ScaleTileBuf; unpacked output is then
 * queued for DMA. g_ScaleParamsPtr is the scaler's parameter block. */
#include "xmen2.h"

void BigAcolyte_RenderScaled(register ScalePicture *pic, register ScaleParams *sp,
	register short vram, char first)
{
	register short size;

	if (first) {
		size = sp->packed ? (long)sp->tilesW << 7 : sp->tilesW * sp->tilesH << 5;
		g_ScaleTileBuf = Mem_StackAlloc(size, STR(Str_Empty_00FFAC, ""));
		Scaler_InitParams();
		g_ScaleParamsPtr = Scaler_Start(sp, 0, pic->pixels, g_ScaleTileBuf, vram);
	}
	g_ScaleParamsPtr = Scaler_Start(sp, 0, pic->pixels, g_ScaleTileBuf, vram);
	Scaler_Step();
	if (!sp->packed)
		VDP_QueueDma(g_ScaleTileBuf, vram, sp->tilesW * sp->tilesH << 5);
}
