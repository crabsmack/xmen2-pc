/* Debris_SetBounceLimit - set how many floor bounces a debris pool's pieces survive.
 *
 * pool: the pool.  n: bounces (at least 1). Each bounce adds 3 to a piece's bounce, so the limit
 * is (n - 1) * 3 (?: as MPW C 3.2.4 and later lower it). */
#include "xmen2.h"

void Debris_SetBounceLimit(DebrisPool *pool, short n)
{
	n = n < 1 ? 1 : n;
	pool->bounceLimit = (n - 1) * 3;
}
