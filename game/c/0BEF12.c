/* SporeMine_UpdatePool - the spore pool's timer (every frame): run each spore in use.
 *
 * A spore without a handler is removed. One outside the deactivation window (relative to the
 * camera, its right and bottom ends 32 px further out) is removed and its spawn record set
 * waiting again (flags 1), so it comes back when the window reaches it. Otherwise: its watch
 * time counts down, its handler runs when the delay has run out (else the delay counts down),
 * and its animation steps (a one-shot script, flags bit 1, ends after its last step). Times
 * count 2 per frame when g_TwoPlayerStep is set. A removed spore's slot holds the last one in
 * use then, so the loop runs that slot again. */
#include "xmen2.h"

#define SCRIPT(s)	((AnimScript *)(u8 *)(s)->anim)	/* the animation's header */

void SporeMine_UpdatePool(void)
{
	register short sx, sy;
	register short step = g_TwoPlayerStep ? 2 : 1;
	register unsigned short i;
	register Spore *s;

	for (s = g_SporeMinePool, i = 0; i < g_SporeMinePoolCount; i++, s++) {
		if (!s->handler) {
			SporeMine_RemoveSpore(s);
			i--;
			s--;
			continue;
		}
		sx = s->x - g_CameraX;
		sy = s->y - g_CameraY;
		if (sx < g_DeactWinLeft || sx > g_DeactWinRight + 0x20 || sy < g_DeactWinTop
		    || sy > g_DeactWinBottom + 0x20) {
			s->rec->flags = 1;
			SporeMine_RemoveSpore(s);
			i--;
			s--;
			continue;
		}
		if (s->watchTime > 0)
			s->watchTime -= step;
		if (s->delay <= 0)
			(*s->handler)(s);
		else
			s->delay -= step;
		if (s->anim) {
			if (s->animTime >= SCRIPT(s)->delay) {
				s->animTime = 0;
				s->animStep++;
				if (SCRIPT(s)->count <= s->animStep) {
					s->animStep = 0;
					if (SCRIPT(s)->flags & 2)
						s->anim = 0;
				}
				if (s->anim)
					s->frame = *(s->anim + (s->animStep << 1) + 7);
			} else
				s->animTime += step;
		}
	}
}
