/* SporeMine_RecIdle - spore handler, its first: play the idle animation (Tbl_SporeMineIdleAnim,
 * its first step's frame shown), then look for a player at once (SporeMine_RecFindTarget).
 *
 * s: the spore. */
#include "xmen2.h"

void SporeMine_RecIdle(register Spore *s)
{
	s->anim = (u8 *)CODEPTR(Tbl_SporeMineIdleAnim);
	s->frame = *(s->anim + 7);
	s->animStep = 0;
	s->animTime = 0;
	s->handler = SporeMine_RecFindTarget;
	s->delay = 0;
}
