/* BioSentinel_StateWalking - BIOSENTINEL walking: 20 px before the end of its segment, stop (no
 * animation, no motion, no pause left) and think (BioSentinel_StateThink) next frame. A player
 * less than 70 px away in x and 40 px in y (body box middles): stop, face the player and swipe
 * (Tbl_Anim_12065E), BioSentinel_StateAfterSwipe when the animation ends. Else walk on while an
 * animation plays, think when none does.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateWalking(register BioSentinel *self)
{
	register XPlayer *pl;
	register short dx;
	register short dy;

	pl = Player_Nearest((Actor *)self);
	dx = (pl->x - self->x) >> 8;
	dy = (((pl->boxTop + pl->boxBottom) << 7) - ((self->boxTop + self->boxBottom) << 7)) >> 8;
	if (Seg_WouldLeaveEnd(self, 20)) {
		self->anim = 0;
		self->ax = 0;
		self->vx = 0;
		if (1) {	/* (a block: MPW then keeps this clear apart from the timer's, as the ROM) */
			self->countdown = 0;
		}
		SET_STATE(self, BioSentinel_StateThink);
		return self->flags & 8;
	}
	if (ABS(dx) < 70 && ABS(dy) < 40) {
		self->ax = 0;
		self->vx = 0;
		if (pl->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_12065E));
		SET_STATE_WAIT(self, BioSentinel_StateAfterSwipe, 0xFF);
		return self->flags & 8;
	}
	if (!self->anim) {
		SET_STATE(self, BioSentinel_StateThink);
		return self->flags & 8;
	}
	SET_STATE(self, BioSentinel_StateWalking);
	return self->flags & 8;
}

