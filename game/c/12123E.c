/* BioSentinel_StateThink - BIOSENTINEL deciding: a player less than 70 px away in x and 40 px in
 * y (body box middles): stop, face the player and swipe (Tbl_Anim_12065E,
 * BioSentinel_StateAfterSwipe when it ends). Pause over (countdown 0): load a volley, stop, face
 * the player and aim (now) up when the player's middle is more than $1E px higher, down when
 * more than 5 px lower, else level. Otherwise walk (Tbl_Anim_12062E) the way it faces at its
 * walking speed (BioSentinel_StateWalking), unless 20 px before the end of its segment: then
 * think again next frame.
 *
 * self: the sentinel. Returns self->flags & 8, or what the aim state returns. */
#include "xmen2.h"

short BioSentinel_StateThink(register BioSentinel *self)
{
	register XPlayer *pl;
	register short dx;
	register short dy;

	pl = Player_Nearest((Actor *)self);
	dx = (pl->x - self->x) >> 8;
	dy = (((pl->boxTop + pl->boxBottom) << 7) - ((self->boxTop + self->boxBottom) << 7)) >> 8;
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
	if (!self->countdown) {
		register long dh;

		dh = ((pl->boxTop + pl->boxBottom) << 7) - ((self->boxTop + self->boxBottom) << 7);
		self->shotsLeft = self->volley;
		self->ax = 0;
		self->vx = 0;
		if (pl->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
		if (dh < -0x1E00) {
			self->state = BioSentinel_StateAimUp;
			return BioSentinel_StateAimUp(self);
		}
		if (dh > 0x500) {
			self->state = BioSentinel_StateAimDown;
			return BioSentinel_StateAimDown(self);
		}
		self->state = BioSentinel_StateAimLevel;
		return BioSentinel_StateAimLevel(self);
	}
	if (Seg_WouldLeaveEnd(self, 20)) {
		SET_STATE(self, BioSentinel_StateThink);
		return self->flags & 8;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_12062E));
	self->vx = self->spr.flags & 0x10 ? -self->walkSpeed : self->walkSpeed;
	SET_STATE(self, BioSentinel_StateWalking);
	return self->flags & 8;
}
