/* Scaler_Start - start a picture-scaling job (Scaler_Step runs it): the work area is allocated on
 * the first job ($DC0 bytes, "SCALER DATA", cut into the six $280-byte lines g_ScalerLine0-5),
 * the parameters copied into g_ScalerParams (unless they are it), the sources, the output buffer
 * and VRAM address kept.
 *
 * params: a ScaleParams record ($26 bytes), or 0 to keep g_ScalerParams.  srcA: a packed 4bpp
 * source (or 0).  srcB: an unpacked one (or 0).  buf: the output.  vram: the output's VRAM
 * address.
 * Source A alone is read at half width (g_ScalerHalfWidth); A and B together: A is expanded
 * into B first (Scaler_ExpandSource) and dropped. With a source the job is ready
 * (g_ScalerReady). Returns &g_ScalerParams when ready, else 0.
 * Hand-written 68000 behind a C frame (dbra copy, sf/st): it can't byte-match. */
#include "xmen2.h"

ScaleParams *Scaler_Start(ScaleParams *params, void *srcA, void *srcB, void *buf, short vram)
{
	if (!g_ScalerWork) {
		u8 *w = (u8 *)Mem_StackAlloc(0xDC0, STR(Str_ScalerData, "SCALER DATA"));

		g_ScalerWork = w;
		g_ScalerLine0 = w;
		g_ScalerLine1 = w + 0x280;
		g_ScalerLine2 = w + 0x500;
		g_ScalerLine3 = w + 0x780;
		g_ScalerLine4 = w + 0xA00;
		g_ScalerLine5 = w + 0xC80;
	}
	if (params && params != &g_ScalerParams)
		g_ScalerParams = *params;
	g_ScalerSrcA = srcA;
	g_ScalerSrcB = srcB;
	g_ScalerDest = buf;
	g_ScalerVram = vram;
	g_ScalerHalfWidth = 0;
	g_ScalerReady = 0;
	if (g_ScalerSrcA) {
		if (!g_ScalerSrcB)
			g_ScalerHalfWidth = 0xFF;
		else {
			Scaler_ExpandSource();
			g_ScalerSrcA = 0;
		}
		g_ScalerReady = 0xFF;
	} else if (g_ScalerSrcB)
		g_ScalerReady = 0xFF;
	return g_ScalerReady ? &g_ScalerParams : 0;
}
