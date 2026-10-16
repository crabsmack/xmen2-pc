/* Mine_StateWatch - mine handler: arm (Mine_StateArm1 from the next frame) when the mine is within
 * 80 px (x) / 40 px (y) of its target's body box; else, once the watch time has run out, pick a
 * target again (Mine_StatePickTarget).
 *
 * m: the mine (target set by Mine_StatePickTarget). */
#include "xmen2.h"

void Mine_StateWatch(register MineRec *m)
{
	register Actor *p = m->target;
	register short left = p->boxLeft - 0x50;
	register short top = p->boxTop - 0x28;
	register short right = p->boxRight + 0x50;
	register short bottom = p->boxBottom + 0x28;

	if (left <= m->x && right >= m->x && top <= m->y && bottom >= m->y) {
		m->handler = Mine_StateArm1;
		m->delay = 0;
	} else if (!m->watchTime) {
		m->handler = Mine_StatePickTarget;
		m->delay = 0;
	}
}
