/* Scaler_InitParams - reset the scaler's parameter block and start a job on g_ScaleTileBuf.
 *
 * Sizes and rectangles 0, clearMode 1 (clear the tile buffer), tableKey 1, no flips (hflip,
 * vflip) or transposition, direct 1 (scale into tiles), not packed, no row budget;
 * g_ScalerTableKey $FF, so the next pass rebuilds its step tables. Then Scaler_Start(
 * &g_ScalerParams, no source A, source B 10, the tile buffer, VRAM 10): the 10 only stands for
 * "a source" (Scaler_Start marks a job ready when it has one); callers start their own job
 * before the scaler runs. g_ScaleParamsPtr = what Scaler_Start returns. Hand-written 68000. */
#include "xmen2.h"

void Scaler_InitParams(void)
{
	g_ScalerParams.srcW = 0;
	g_ScalerParams.srcH = 0;
	g_ScalerParams.srcX = 0;
	g_ScalerParams.srcY = 0;
	g_ScalerParams.rectW = 0;
	g_ScalerParams.rectH = 0;
	g_ScalerParams.tilesW = 0;
	g_ScalerParams.tilesH = 0;
	g_ScalerParams.outW = 0;
	g_ScalerParams.outH = 0;
	g_ScalerParams.clearMode = 1;
	g_ScalerParams.tableKey = 1;
	g_ScalerParams.hflip = 0;
	g_ScalerParams.vflip = 0;
	g_ScalerParams.transpose = 0;
	g_ScalerParams.direct = 1;
	g_ScalerParams.packed = 0;
	g_ScalerParams.rowsPerCall = 0;
	g_ScalerTableKey = 0xFF;
	g_ScaleParamsPtr = Scaler_Start(&g_ScalerParams, 0, AT68(void, 10), g_ScaleTileBuf, 10);
}
