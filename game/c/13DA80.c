/* ExplodeFx_BurstUpdate - a Burst's timer, once a frame: each live puff moves (speed, then
 * acceleration; twice in two-player stepping) and animates (6 frames per picture, 2 per frame in
 * two-player stepping); after its 7th picture it is done, a type-2/3 puff dropping a segment
 * power-up 16 px right of and below it (SegmentPlusPowerUp_Spawn kind 0 / $F0). With no puff
 * left the effect ends (ExplodeFx_Free).
 *
 * fx: the effect (its 10 FxPuff follow the header). */
#include "xmen2.h"

void ExplodeFx_BurstUpdate(register ExplodeFx *fx)
{
	register FxPuff *p;
	register char live;
	short i;
	register short ticks, frame;

	i = 0;
	p = (FxPuff *)(fx + 1);
	live = 0;
	if (fx->soundDelay > 0)
		fx->soundDelay--;
	for (;; i++, p++) {
		if (i >= 10)
			break;
		if (p->type) {
			p->x += p->vx;
			p->y += p->vy;
			p->vx += p->ax;
			p->vy += p->ay;
			if (g_TwoPlayerStep) {
				p->x += p->vx;
				p->y += p->vy;
				p->vx += p->ax;
				p->vy += p->ay;
			}
			ticks = p->anim & 0xF;
			frame = p->anim >> 4;
			ticks -= g_TwoPlayerStep ? 2 : 1;
			if (ticks <= 0) {
				frame++;
				if (frame > 6) {
					if (p->type == 2)
						SegmentPlusPowerUp_Spawn(p->x + 0x1000, p->y + 0x1000, 0);
					else if (p->type == 3)
						SegmentPlusPowerUp_Spawn(p->x + 0x1000, p->y + 0x1000, 0xF0);
					p->type = 0;
					continue;
				}
				p->anim = frame << 4 | 6;
			} else {
				p->anim &= 0xF0;
				p->anim |= ticks & 0xF;
			}
			live = 1;
		}
	}
	if (!live)
		ExplodeFx_Free(fx);
}
