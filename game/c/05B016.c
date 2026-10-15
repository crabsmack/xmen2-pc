/* Boulder_InitDebrisPool - make buf the pool of 30 rock-debris records (g_BoulderDebrisPool),
 * all zero, none in use.
 *
 * buf: $258 bytes (Boulder_ClassInit's Mem_StackAlloc).
 * MPW: the original cleared the last long (frame, big, floorY) with one long store (move.l d0:
 * a 4-byte field set to 0), as if its record had a long there; the PC clears the three fields. */
#include "xmen2.h"

void Boulder_InitDebrisPool(register BoulderDebris *buf)
{
	register short i;

	g_BoulderDebrisCount = 0;
	g_BoulderDebrisPool = buf;
	for (i = 0; i < 30; i++, buf++) {
		buf->x = 0;
		buf->y = 0;
		buf->vx = 0;
		buf->vy = 0;
		buf->ax = 0;
		buf->ay = 0;
#ifdef XMEN2_MPW
		*(s32 *)&buf->frame = 0;
#else
		buf->frame = 0;
		buf->big = 0;
		buf->floorY = 0;
#endif
	}
}
