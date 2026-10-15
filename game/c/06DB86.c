/* Brood_StateWalkToPlayer - BROOD state: walk until a live player's body box touches the box
 * +-$A0 x, +-$32 y around the object (that player becomes the target, next AI-script step);
 * at an end of the patrol range stand for 20 frames and charge again (Brood_StateLimitWait).
 *
 * self: the object. Returns flags & 8 when it stops at a limit, nothing otherwise. */
#include "xmen2.h"

short Brood_StateWalkToPlayer(Brood *self)
{
	short left = (self->x >> 8) - 0xA0;
	short top = (self->y >> 8) - 0x32;
	short right = left + 0x140;
	short bottom = top + 0x64;
	long i;
	XPlayer *p;
	short x;

	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0F0));
	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2)) && left <= p->boxRight
		    && right >= p->boxLeft && top <= p->boxBottom && bottom >= p->boxTop) {
			self->target = p;
			AI_NEXT_STEP(self, self->ai, 0);
			RETURN_NO_VALUE;
		}
	}
	x = self->x >> 8;
	if (x <= self->xMin || x >= self->xMax) {
		self->countdown = 20;
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
		SET_STATE(self, Brood_StateLimitWait);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
