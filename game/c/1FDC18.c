/* Scroll_UpdateParallax - the frame's parallax: with a parallax set up (g_ParallaxDesc), the band
 * scroll values are recomputed (Scroll_ComputeParallax) and sent at the next V-blank: line
 * scroll as a DMA of the H-scroll buffer ($380 bytes to VRAM $F400) and the vertical scroll
 * (Scroll_RequestVsram); column scroll ('2' descriptors) as Scroll_RequestColumnVs. */
#include "xmen2.h"

void Scroll_UpdateParallax(void)
{
	register u32 *buf = g_HScrollBuf;

	if (g_ParallaxDesc) {
		Scroll_ComputeParallax();
		if ((s8)g_ParallaxDesc->type != '2') {
			VDP_QueueDma(buf, 0xF400, 0x380);
			Scroll_RequestVsram();
		} else
			Scroll_RequestColumnVs();
	}
}
