/* Rand_Next - the next pseudo-random number, 0..$FFF.
 *
 * Three lagged tables of words (233, 151 and 127 entries) are each XORed into the output
 * before last (g_RandPrev2); each table's byte index steps down by 2 per call and wraps. The
 * result is remembered (g_RandPrev, the old one moving to g_RandPrev2) and its low 12 bits
 * returned. Hand-written 68000, straight-line code: the second and third steps and the end are
 * the fragments Rand_Next_Mix2, Rand_Next_Mix3 and Rand_Next_Finish. */
#include "xmen2.h"

short Rand_Next(void)
{
	short v;
	short prev;

	v = g_RandPrev2;
	v ^= ((s16 *)Tbl_RandXor1)[g_RandIdx1 >> 1];
	g_RandIdx1 -= 2;
	if (g_RandIdx1 < 0)
		g_RandIdx1 = 0x1D0;
	/* Rand_Next_Mix2 */
	v ^= ((s16 *)Tbl_RandXor2)[g_RandIdx2 >> 1];
	g_RandIdx2 -= 2;
	if (g_RandIdx2 < 0)
		g_RandIdx2 = 0x12C;
	/* Rand_Next_Mix3 */
	v ^= ((s16 *)Tbl_RandXor3)[g_RandIdx3 >> 1];
	g_RandIdx3 -= 2;
	if (g_RandIdx3 < 0)
		g_RandIdx3 = 0xFC;
	/* Rand_Next_Finish */
	prev = g_RandPrev;
	g_RandPrev = v;
	g_RandPrev2 = prev;
	return v & 0xFFF;
}
