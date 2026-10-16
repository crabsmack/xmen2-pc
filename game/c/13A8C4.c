/* Mine_PoolUpdate - the mine pool's timer (every frame): run each mine in use.
 *
 * A mine without a handler is removed. One outside the deactivation window (relative to the
 * camera, its right and bottom ends 32 px further out) is removed and its spawn record set
 * waiting again (flags 1), so it comes back when the window reaches it. Otherwise: its watch
 * time counts down, its handler runs when the delay has run out (else the delay counts down),
 * and its animation steps (a one-shot script, flags bit 1, ends after its last step). Times
 * count 2 per frame when g_TwoPlayerStep is set. A removed mine's slot holds the last one in
 * use then, so the loop runs that slot again. */
#include "xmen2.h"

#define SCRIPT(m)	((AnimScript *)(u8 *)(m)->anim)	/* the animation's header */

void Mine_PoolUpdate(void)
{
	register short sx, sy;
	register short step = g_TwoPlayerStep ? 2 : 1;
	register unsigned short i;
	register MineRec *m;

	for (m = g_MinePool, i = 0; i < g_MinePoolCount; i++, m++) {
		if (!m->handler) {
			Mine_PoolRemove(m);
			i--;
			m--;
			continue;
		}
		sx = m->x - g_CameraX;
		sy = m->y - g_CameraY;
		if (sx < g_DeactWinLeft || sx > g_DeactWinRight + 0x20 || sy < g_DeactWinTop
		    || sy > g_DeactWinBottom + 0x20) {
			m->rec->flags = 1;
			Mine_PoolRemove(m);
			i--;
			m--;
			continue;
		}
		if (m->watchTime > 0)
			m->watchTime -= step;
		if (m->delay <= 0)
			(*m->handler)(m);
		else
			m->delay -= step;
		if (m->anim) {
			if (m->animTime >= SCRIPT(m)->delay) {
				m->animTime = 0;
				m->animStep++;
				if (SCRIPT(m)->count <= m->animStep) {
					m->animStep = 0;
					if (SCRIPT(m)->flags & 2)
						m->anim = 0;
				}
				if (m->anim)
					m->frame = *(m->anim + (m->animStep << 1) + 7);
			} else
				m->animTime += step;
		}
	}
}
