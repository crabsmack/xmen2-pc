/* Scaler_ExpandSource - unpack a 4bpp source picture into one byte per pixel.
 *
 * Reads g_ScalerSrcA (two pixels per byte, rows of (srcW + 1) / 2 bytes) and writes each byte
 * as Tbl_PixelPairExpand's word to g_ScalerSrcB (each pixel doubled into a full byte: $hl ->
 * $hh $ll), for the srcH rows of g_ScalerParams. Scaler_Start does this when it is given both
 * sources. The byte count is the low word of the product, as in the 68000 (0 would be 65536).
 * Hand-written 68000. */
#include "xmen2.h"

void Scaler_ExpandSource(void)
{
	u8 *src = g_ScalerSrcA;
	u16 *dst = g_ScalerSrcB;
	unsigned short n;

	n = (unsigned short)((unsigned long)((unsigned short)(g_ScalerParams.srcW + 1) >> 1)
	                     * (unsigned short)g_ScalerParams.srcH);
	do
		*dst++ = ((u16 *)Tbl_PixelPairExpand)[*src++];
	while (--n);
}
