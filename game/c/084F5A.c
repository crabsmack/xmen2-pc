/* SmallBrood_StateWaitForPlayer - SMALLBROOD state (kamikaze variant, set by SmallBrood_Init):
 * stand until a live player's body box touches the box +-$D4 x, +-$50 y around the object,
 * then reload the cooldown and start the AI script at step 0.
 *
 * self: the object. Both players are checked; when both are in reach the script restarts
 * twice in the same frame (harmless: same result). */
#include "xmen2.h"

void SmallBrood_StateWaitForPlayer(SmallBrood *self)
{
	short left = (self->x >> 8) - 0xD4;
	short top = (self->y >> 8) - 0x50;
	short right = left + 0x1A8;
	short bottom = top + 0xA0;
	long i;
	XPlayer *p;

	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim7));
	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p)) && left <= p->boxRight
		    && right >= p->boxLeft && top <= p->boxBottom && bottom >= p->boxTop) {
			self->cooldown = self->cooldownTime;
			AI_FIRST_STEP(self, self->ai, 0);
		}
	}
}
