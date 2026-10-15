/* SiberiaB1_SpawnDebris - throw 4 ice shards from (x, y) (as many as the pool has free): each
 * within 4 px of it, with a random velocity (each axis -$1FF..$1FF), gravity $20, a random first
 * frame set (0 or 3), frame, delay and age.
 *
 * x, y: pixels (the middle of a broken ice block: SiberiaB1_IceTileHit). */
#include "xmen2.h"

void SiberiaB1_SpawnDebris(register short x, register short y)
{
	register IceShard *p;
	register short i;

	for (i = 0; i < 4; i++) {
		if ((p = SiberiaB1_DebrisAlloc()) == 0)
			break;
		p->x = (x << 8) + (((unsigned short)Rand_Next() & 0x3FF) - ((unsigned short)Rand_Next() & 0x3FF));
		p->y = (y << 8) + (((unsigned short)Rand_Next() & 0x3FF) - ((unsigned short)Rand_Next() & 0x3FF));
		p->vx = ((unsigned short)Rand_Next() & 0x1FF) - ((unsigned short)Rand_Next() & 0x1FF);
		p->vy = ((unsigned short)Rand_Next() & 0x1FF) - ((unsigned short)Rand_Next() & 0x1FF);
		if (p->vx > 0)
			p->ax = 2;
		else
			p->ax = -2;
		p->ay = 0x20;
		p->frameSet = ((unsigned short)Rand_Next() & 1) ? 0 : 3;
		p->frame = (unsigned short)Rand_Next() & 1;
		p->frameDelay = (unsigned short)Rand_Next() & 3;
		p->age = (unsigned short)Rand_Next() & 0x3F;
	}
}
