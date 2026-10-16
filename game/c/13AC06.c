/* Mine_StatePickTarget - mine handler: watch the nearest live player (Player_NearestLive) for 60
 * frames (Mine_StateWatch at once); with none, try again next frame.
 *
 * m: the mine. It is passed to Player_NearestLive as if it were an object. */
#include "xmen2.h"

void Mine_StatePickTarget(register MineRec *m)
{
	register Actor *p;

	if (p = m->target = Player_NearestLive(m)) {
		m->watchTime = 0x3C;
		m->handler = Mine_StateWatch;
		m->delay = 0;
	}
}
