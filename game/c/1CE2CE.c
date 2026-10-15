/* Magneto_StartShot - MAGNETO on the ground: the wind-up of a shot in direction aimDir; the shot
 * itself (Magneto_FireShot) when the animation ends.
 *
 * self: the player.
 * Stops in x, B unlatched; crouching (px, +$10) the crouched wind-up Tbl_MagnetoAnim7, else
 * Magneto_ShotAnim for the direction.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Magneto_StartShot(register Magneto *self)
{
	register void *anim;

	self->ax = 0;
	self->vx = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	if (self->px)
		anim = CODEPTR(Tbl_MagnetoAnim7);
	else
		anim = Magneto_ShotAnim(self->aimDir);
	Obj_SetAnim(self, anim);
	SET_STATE_WAIT(self, (StateFn)Magneto_FireShot, 0xFF);
	return self->flags & 8;
}
