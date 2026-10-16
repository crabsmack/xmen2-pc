/* BrainRocket_Attack - BRAINROCKET Attack (class slot +$22): hit the target (its TakeDamage) with
 * the hit it was given, no hit cooldown, then blow up: the camera shakes (Tbl_BrainRocketQuake3
 * on a vertical heading, 90 or 270, else Tbl_BrainRocketQuake), an explosion at its body box's
 * centre, Kill (slot +$52) and the explosion sound ($38, restarted).
 *
 * self: the rocket.  target: what it touched.  type, damage: the hit.  hx, hy: the hit point. */
#include "xmen2.h"

void BrainRocket_Attack(BrainRocket *self, Actor *target, char type, char damage, short hx, short hy)
{
	(*target->cls->method[8])(target, self, type, damage, hx, hy);	/* +$26 TakeDamage */
	if (self->angle == 90 || self->angle == 270)
		g_CamShake = CODEPTR(Tbl_BrainRocketQuake3);
	else
		g_CamShake = CODEPTR(Tbl_BrainRocketQuake);
	Explosion_Spawn((self->boxRight + self->boxLeft) << 7, (self->boxTop + self->boxBottom) << 7, 0);
	(*self->cls->method[19])(self);					/* +$52 Kill */
	Snd_Stop(0x38);
	Snd_Play(0x38);
}
