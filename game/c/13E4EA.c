/* ExplodeFx_BounceUpdate - a Bounce's timer, once a frame: each released, moving puff of the 5
 * groups bounces off the screen's edges (x outside 0..$120: vx negated, else y outside 0..$C0:
 * vy negated), moves (twice in two-player stepping) and animates (7 pictures of 15 frames, 2 per
 * frame in two-player stepping), stopping after the 7th; while a group has fewer than 3 puffs
 * out, the next is released once its first has run 3 frames per puff out. With nothing moving
 * the effect ends (ExplodeFx_Free); else a bounce plays the explosion sound (when 6 frames have
 * passed since the last).
 *
 * fx: the effect (its 5 FxGroup follow the header).
 * A flag cleared for every moving puff is never read (the ROM keeps the store). */
#include "xmen2.h"

void ExplodeFx_BounceUpdate(register ExplodeFx *fx)
{
	short n;
	short elapsed;
	char live;
	char unused;
	short ticks;
	char bounced;
	register FxGroup *g;
	short j;
	register short sx, sy;
	short frame;

	n = 0;
	g = (FxGroup *)(fx + 1);
	live = 0;
	bounced = 0;
	if (fx->soundDelay > 0)
		fx->soundDelay--;
	for (;; n++, g++) {
		if (n >= 5)
			break;
		if (!g->count)
			continue;
		for (j = 0; j < g->count; j++) {
			if (!g->vx[j] && !g->vy[j])
				continue;
			unused = 0;
			sx = (g->x[j] >> 3) - g_CameraX;
			sy = (g->y[j] >> 3) - g_CameraY;
			if (sx > 0x120 || sx < 0) {
				g->vx[j] = -g->vx[j];
				bounced = 1;
			} else if (sy > 0xC0 || sy < 0) {
				g->vy[j] = -g->vy[j];
				bounced = 1;
			}
			g->x[j] += g->vx[j];
			g->y[j] += g->vy[j];
			if (g_TwoPlayerStep) {
				g->x[j] += g->vx[j];
				g->y[j] += g->vy[j];
			}
			ticks = g->anim[j] & 0xF;
			frame = g->anim[j] >> 4;
			ticks -= g_TwoPlayerStep ? 2 : 1;
			if (g->count < 3) {
				elapsed = frame * 15 + (15 - ticks);
				if (g->count * 3 <= elapsed)
					g->count++;
			}
			if (ticks <= 0) {
				frame++;
				if (frame > 6) {
					g->vx[j] = 0;
					g->vy[j] = 0;
					continue;
				}
				g->anim[j] = frame << 4 | 0xF;
			} else {
				g->anim[j] &= 0xF0;
				g->anim[j] |= ticks & 0xF;
			}
			live = 1;
		}
	}
	if (!live)
		ExplodeFx_Free(fx);
	else if (bounced && fx->soundDelay <= 0) {
		Snd_Stop(0x38);
		Snd_Play(0x38);
		fx->soundDelay = 6;
	}
}
