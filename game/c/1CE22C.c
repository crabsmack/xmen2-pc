/* Magneto_FireShot - MAGNETO on the ground at the end of the shot wind-up (Magneto_StartShot):
 * the globe flies in direction aimDir, then the follow-through.
 *
 * self: the player.
 * The globe: Magneto_SpawnShot, fired by the player, crouching when px (+$10, the crouch flag of
 * the ground attacks: Magneto_StateCrouchAim) is set, in front or behind as his sprite, not in
 * the air, in the 7 tiles below his player tiles' end (spr.tileBase + g_PlayerTiles - 7).
 * Crouching: the crouched follow-through (Tbl_MagnetoAnim8) and XPlayer_CrouchAttackRecover,
 * else Magneto_ShotEndAnim and XPlayer_AttackRecover; the state runs when the animation ends
 * (timer $FF). */
#include "xmen2.h"

void Magneto_FireShot(register Magneto *self)
{
	register MagneticGlob *shot;
	register void *anim;

	shot = Magneto_SpawnShot(self->x, self->y, self->aimDir, 1, self->px,
		self->spr.flags & 4 ? 1 : 0, 0, self->spr.tileBase + g_PlayerTiles - 7);
	if (self->px) {
		anim = CODEPTR(Tbl_MagnetoAnim8);
		self->state = (StateFn)XPlayer_CrouchAttackRecover;
	} else {
		anim = Magneto_ShotEndAnim(self->aimDir);
		self->state = (StateFn)XPlayer_AttackRecover;
	}
	Obj_SetAnim(self, anim);
	self->timer = 0xFF;
}
