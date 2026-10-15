/* Magneto_StartAirShot - MAGNETO: start a shot in the air in direction aimDir (the air attacks
 * set it): the wind-up animation (Magneto_AirShotAnim), then Magneto_StateAirShot now.
 *
 * self: the player.
 * Whether he is hovering (the hover animation Tbl_MagnetoAnim playing) is kept in py (+$12,
 * which nothing but Actor_Init sets otherwise), so the hover resumes after the shot
 * (Magneto_StateAirShotEnd); when not hovering he falls with normal gravity meanwhile. B is
 * unlatched. The state change is not the GOTO_STATE macro: no argument pop between the
 * calls, all of them at the end. */
#include "xmen2.h"

void Magneto_StartAirShot(register Magneto *self)
{
	register void *anim;

	if (self->anim == CODEPTR(Tbl_MagnetoAnim))
		self->py = 1;
	else
		self->py = 0;
	if (!self->py)
		self->ay = self->charRec->gravity;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	anim = Magneto_AirShotAnim(self->aimDir);
	Obj_SetAnim(self, anim);
	if (1) {	/* (a block under a constant condition: MPW pops the arguments at its end) */
		self->state = Magneto_StateAirShot;
		Magneto_StateAirShot(self);
	}
}
