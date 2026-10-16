/* ExplodeFx_ChainStep - place a chain's next explosions: at the path's end, start it again (one
 * repeat less unless they are endless, after repeatDelay frames) or end the path (step 0); then,
 * while the delay allows, each step takes a free explosion slot of the 27 and puts it at the
 * origin + (dx, -dy) (dx negated when h-flipped), the step's delay becoming the chain's. The
 * explosion sound plays for new explosions (when 6 frames have passed since the last).
 *
 * fx: the chain (its 27 FxBlast follow the header).
 * With no free slot nothing is placed (the step stays for the next frame). */
#include "xmen2.h"

void ExplodeFx_ChainStep(ExplodeFxChain *fx)
{
	register FxBlast *b;
	register short i;
	register char spawned;
	register FxChainStep *s;

	b = (FxBlast *)(fx + 1);
	i = 0;
	spawned = 0;
	if (fx->step->delay < 0) {
		if (fx->repeats) {
			if (fx->repeats > 0)
				fx->repeats--;
			fx->step = fx->path;
			fx->delay = fx->repeatDelay;
		} else {
			fx->step = 0;
			return;
		}
	}
	if (fx->delay > 0)
		return;
	for (;; i++, b++) {
		if (i >= 27)
			break;
		if (b->live)
			continue;
		s = fx->step;
		fx->delay = s->delay;
		b->x = fx->x + (!(fx->chainFlags & 2) ? s->dx : -s->dx);
		b->y = fx->y - s->dy;
		b->anim = 6;
		b->live = 1;
		spawned = 1;
		fx->step++;
		if (fx->delay > 0)
			break;
		if (fx->step->delay < 0) {
			if (fx->repeats) {
				if (fx->repeats > 0)
					fx->repeats--;
				fx->step = fx->path;
				fx->delay = fx->repeatDelay;
			} else {
				fx->step = 0;
				return;
			}
		}
		if (fx->delay > 0)
			break;
	}
	if (spawned && fx->soundDelay <= 0) {
		Snd_Stop(0x38);
		Snd_Play(0x38);
		fx->soundDelay = 6;
	}
}
