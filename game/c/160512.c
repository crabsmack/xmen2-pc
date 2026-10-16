/* Rand_Seed - seed Rand_Next: the seed's bits become the three table indexes and the last output.
 *
 * seed: any word.
 * Bits 0-4 (times 4) index the third table (g_RandIdx3), bits 5-9 (times 4) the second
 * (g_RandIdx2), bits 10-15 (times 2) the first (g_RandIdx1); g_RandPrev = seed and g_RandPrev2
 * = 0. Hand-written 68000 behind a C frame, like Rand_Next (andi.w, add.w d0,d0 twice, rol.w #7:
 * nothing MPW C emits), so it can't byte-match. */
#include "xmen2.h"

void Rand_Seed(short seed)
{
	g_RandIdx3 = (seed & 0x1F) << 2;
	g_RandIdx2 = (seed & 0x3E0) >> 3;
	g_RandIdx1 = (unsigned short)(seed & 0xFC00) >> 9;
	g_RandPrev = seed;
	g_RandPrev2 = 0;
}
