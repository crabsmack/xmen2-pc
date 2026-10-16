/* SporeMine_RecWatch - spore handler: arm (SporeMine_RecArm1 at once) when the spore is within
 * 80 px (x) / 40 px (y) of its target's body box; else, once the watch time has run out, pick
 * a target again (SporeMine_RecFindTarget).
 *
 * s: the spore (target set by SporeMine_RecFindTarget). */
#include "xmen2.h"

void SporeMine_RecWatch(register Spore *s)
{
	register Actor *p = s->target;
	register short left = p->boxLeft - 0x50;
	register short top = p->boxTop - 0x28;
	register short right = p->boxRight + 0x50;
	register short bottom = p->boxBottom + 0x28;

	if (left <= s->x && right >= s->x && top <= s->y && bottom >= s->y) {
		s->handler = SporeMine_RecArm1;
		s->delay = 0;
	} else if (!s->watchTime) {
		s->handler = SporeMine_RecFindTarget;
		s->delay = 0;
	}
}
