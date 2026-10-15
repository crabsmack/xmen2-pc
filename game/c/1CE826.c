/* Magneto_SpawnBomb - MAGNETO at the end of the bomb wind-up (Magneto_StartBomb): the flux bomb
 * appears at his hand and is held (Magneto_StateHoldBomb).
 *
 * self: the player.
 * In the air: the control state (slot +$A6) from the next frame. The bomb: Magneto_SpawnFluxBomb
 * in direction aimDir, thrown by the player, crouching when px (+$10), strong with health above
 * $700; kept in bomb. None: sound $83 and the control state from the next frame. Else it is held
 * (held 1), the follow-through (crouching Tbl_MagnetoAnim8, else Magneto_ShotEndAnim) and
 * Magneto_StateHoldBomb from the next frame.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Magneto_SpawnBomb(register Magneto *self)
{
	register void *anim;

	if (self->flags & 0x1000) {
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	self->bomb = Magneto_SpawnFluxBomb(self->x, self->y, self->aimDir, 1, self->px,
		self->health >= 0x701);
	if (!self->bomb) {
		Snd_Stop(0x83);
		Snd_Play(0x83);
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	self->bomb->held = 1;
	if (self->px)
		anim = CODEPTR(Tbl_MagnetoAnim8);
	else
		anim = Magneto_ShotEndAnim(self->aimDir);
	Obj_SetAnim(self, anim);
	SET_STATE(self, Magneto_StateHoldBomb);
	return self->flags & 8;
}
