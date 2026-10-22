/* VDP_QueueDma - queue a VRAM DMA for the next V-blank, or do it now if the display is off.
 *
 * src: source in 68000 memory.  dest: VRAM address (low word).  len: bytes (low word).
 * Display off (g_VdpReg1 bit 6 clear): VDP_DmaToVram at once. Otherwise, with interrupts
 * masked, a request that continues the last queued one in both source and VRAM lengthens it;
 * else it gets a new DmaEntry in g_DmaQueue. An entry whose source would cross a 64 KB
 * boundary ends there and the rest becomes the next entry. VDP_FlushDmaQueue runs them.
 * Hand-written 68000; the split rewrites the caller's argument slots, which the C does with
 * its parameters. */
#include "xmen2.h"

void VDP_QueueDma(void *src, long dest, long len)
{
	unsigned long from = ADDR68(src);
	DmaEntry *e;
	DmaEntry *prev;
	unsigned long end;
	short n;	/* the entry's length in bytes */

#if defined(XMEN2_NATIVE)
	/* phase 7: a player 3-4's upload into player 2's VRAM areas (its character, its HUD) is its own
	   tiles' (the platform copies it now; they are shown from there) */
	if (g_P34Active && plat_p34_dma(from, (unsigned short)dest, (unsigned short)len))
		return;
#endif
	if (!(g_VdpReg1 & 0x40)) {
		VDP_DmaToVram(from, dest, len);
		return;
	}
	plat_set_sr(plat_sr() | 0x0700);
	e = &g_DmaQueue[g_DmaQueueCount];
	if (g_DmaQueueCount != 0) {
		prev = e - 1;
		if ((unsigned short)prev->len + prev->src == from
		    && (unsigned short)(prev->len + prev->dest) == (unsigned short)dest) {
			n = prev->len + (short)len;
			e = prev;
			goto split;
		}
	}
	for (;;) {
		g_DmaQueueCount++;
		e->regMode = 0x8174;
		e->src = from;
		e->regSrcLo = 0x9500 | (from >> 1 & 0xFF);
		e->regSrcMid = 0x9600 | (from >> 9 & 0xFF);
		e->regSrcHi = 0x9700 | (from >> 17 & 0x7F);
		e->dest = (unsigned short)dest;
		e->cmd = (unsigned long)(0x4000 | (e->dest & 0x3FFF)) << 16 | 0x80 | e->dest >> 14;
		n = (short)len;
	split:
		end = e->src + n;
		if (end >> 16 == e->src >> 16 || (end & 0xFFFF) == 0)
			break;
		/* the rest, from the boundary on, is the next request */
		n -= (short)end;
		len = (unsigned short)end;
		from = end & 0xFFFF0000;
		dest = (unsigned short)(e->dest + n);
		e->len = n;
		e->regLenLo = 0x9300 | ((unsigned short)n >> 1 & 0xFF);
		e->regLenHi = 0x9400 | (unsigned short)n >> 9;
		e++;
	}
	e->len = n;
	e->regLenLo = 0x9300 | ((unsigned short)n >> 1 & 0xFF);
	e->regLenHi = 0x9400 | (unsigned short)n >> 9;
	plat_set_sr(plat_sr() & 0xF8FF);
}
