/* ExplodeFx_TrailUpdate - a Trail's timer, once a frame. The head (puff 0, invisible) moves by
 * its speed and acceleration and counts its steps (anim); every 2 * n steps (n = puffs dropped
 * so far, its type, up to 10) it drops the next puff where it is, with the explosion sound
 * (when 6 frames have passed since the last). It stops once outside the deactivation window
 * (g_DeactWin*, relative to the screen). The dropped puffs animate (7 pictures of 6 frames, 2
 * per frame in two-player stepping) and are done after the 7th. With nothing moving or
 * animating the effect ends (ExplodeFx_Free).
 *
 * fx: the effect (its 10 FxPuff follow the header). */
#include "xmen2.h"

void ExplodeFx_TrailUpdate(register ExplodeFx *fx)
{
	FxPuff *head;
	char live;
	short ticks;
	short frame;
	register FxPuff *p;
	short i;
	char out;
	register short sx, sy;

	i = 0;
	p = (FxPuff *)(fx + 1);
	head = p;
	live = 0;
	if (fx->soundDelay > 0)
		fx->soundDelay--;
	for (;; i++, p++) {
		if (i >= head->type)
			break;
		if (!p->type)
			continue;
		if (i == 0) {
			out = 0;
			sx = (p->x >> 8) - g_CameraX;
			sy = (p->y >> 8) - g_CameraY;
			if (sx > g_DeactWinRight)
				out = 1;
			else if (sx < g_DeactWinLeft)
				out = 1;
			else if (sy > g_DeactWinBottom)
				out = 1;
			else if (sy < g_DeactWinTop)
				out = 1;
			if (out)
				continue;
		}
		if (i == 0) {
			p->x += p->vx;
			p->y += p->vy;
			p->vx += p->ax;
			p->vy += p->ay;
			p->anim++;
			if (p->type < 10 && p->anim >= p->type * 2) {
				if (fx->soundDelay <= 0) {
					Snd_Stop(0x38);
					Snd_Play(0x38);
					fx->soundDelay = 6;
				}
				head[head->type].x = p->x;
				head[head->type].y = p->y;
				p->type++;
			}
		} else {
			ticks = p->anim & 0xF;
			frame = p->anim >> 4;
			ticks -= g_TwoPlayerStep ? 2 : 1;
			if (ticks <= 0) {
				frame++;
				if (frame > 6) {
					p->type = 0;
					continue;
				}
				p->anim = frame << 4 | 6;
			} else {
				p->anim &= 0xF0;
				p->anim |= ticks & 0xF;
			}
		}
		live = 1;
	}
	if (!live)
		ExplodeFx_Free(fx);
}
