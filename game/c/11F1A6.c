/* BioBalls_AllocBallList - allocate the ball list the BIOBALLS objects share (g_BioBallsList)
 * for n balls, all gone (-1), and build the ball sine table (g_BioBallsSine: sin of 0..179
 * degrees in 128 steps, 16 fraction bits).
 *
 * n: balls.
 * Returns the list: the count, then n balls of 6 bytes (Mem_StackAlloc). */
#include "xmen2.h"

s16 *BioBalls_AllocBallList(short n)
{
	short size, i;
	s32 *p;

	BioBalls_EmptyStub();
	size = n * 6 + 2;
	g_BioBallsList = (s16 *)Mem_StackAlloc(size, STR(Str_Empty_11F22A, ""));
	Mem_Set(g_BioBallsList, 0xFF, size);
	*g_BioBallsList = n;
	p = g_BioBallsSine;
	for (i = 0; i < 128; i++, p++)
		*p = Math_SinDeg(i * 180 / 128);
	return g_BioBallsList;
}
