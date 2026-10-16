/* SporeMine_RecFindTarget - spore handler: watch the nearest live player (Player_NearestLive)
 * for 60 frames (SporeMine_RecWatch at once); with none, try again next frame.
 *
 * s: the spore. It is passed to Player_NearestLive as if it were an object. */
#include "xmen2.h"

void SporeMine_RecFindTarget(register Spore *s)
{
	register Actor *p;

	if (p = s->target = Player_NearestLive(s)) {
		s->watchTime = 0x3C;
		s->handler = SporeMine_RecWatch;
		s->delay = 0;
	}
}
