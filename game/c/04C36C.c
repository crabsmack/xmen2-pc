/* Apocalypse_StateHurt - APOCALYPSE hurt state (+$60, after a hit): the hurt pose (frames 5 and 6
 * play Tbl_ApocalypseAnim4, others show frame 7), 1000 frames of hit cooldown, 20 frames
 * countdown, stop circling, moving and all motion, sound $EB. Out of health: defeated
 * (Apocalypse_StateDefeated) in 20 frames; else knocked back at $800 away from where it faces
 * (Apocalypse_StateKnockback).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateHurt(register Apocalypse *self)
{
	switch (self->spr.frame) {
	case 5:
	case 6:
		Obj_SetAnim(self, CODEPTR(Tbl_ApocalypseAnim4));
		break;
	default:
		self->anim = 0;
		(*self->cls->method[20])(self, 7);		/* +$56 SetFrame */
		break;
	}
	self->hitCooldown = 1000;
	self->countdown = 20;
	self->moveFlags &= ~1;
	self->moveFlags &= ~0x10;
	STOP_MOVING(self);
	Snd_Stop(0xEB);
	Snd_Play(0xEB);
	if (self->health <= 0) {
		SET_STATE_WAIT(self, Apocalypse_StateDefeated, 20);
		return self->flags & 8;
	}
	self->vx = -(self->spr.flags & 0x10 ? -0x800L : 0x800L);
	SET_STATE(self, Apocalypse_StateKnockback);
	return self->flags & 8;
}
