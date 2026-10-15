/* BanimazaDoor_InitDebris - throw the shards of a shattered door: each starts at a random point
 * around (x, y) (x +-$1060, y +-$20C0, 8 fraction bits) with a random velocity (vx about 600,
 * vy about -1700: up) and acceleration (ax about -10, ay about $46: gravity); mirrored in x
 * when the attacker was to the right.
 *
 * s: the shards.  n: how many (at least 1).  x, y: the centre, 8 fraction bits.
 * mirror: non-zero: fly to the left. */
#include "xmen2.h"

void BanimazaDoor_InitDebris(register DoorShard *s, register short n, register long x, register long y, register unsigned char mirror)
{
	do {
		s->x = x + (((unsigned short)Rand_Next() - 0x830) << 1);
		s->y = y + (((unsigned short)Rand_Next() - 0x830) << 2);
		s->vx = (((unsigned short)Rand_Next() - 0x768) >> 2) + 600;
		s->vy = (((unsigned short)Rand_Next() - 0x768) >> 4) + -1700;
		s->ax = (((unsigned short)Rand_Next() - 0x768) >> 10) + -10;
		s->ay = (((unsigned short)Rand_Next() - 0x768) >> 8) + 0x46;
		if (mirror) {
			s->vx = -s->vx;
			s->ax = -s->ax;
		}
		s++;
		--n;
	} while (n);
}
