/* Tusk_HurtFlash - TUSK hit by hit type $D-$F (Tusk_TakeDamage): sound $36, leave the enemy body
 * hit list, and flash for 50 frames (Tusk_StateHurtFlash).
 *
 * self: TUSK.
 * Returns 0. Never runs in the corpus. */
#include "xmen2.h"

short Tusk_HurtFlash(Tusk *self)
{
	Snd_Stop(0x36);
	Snd_Play(0x36);
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	self->hittable = 0;
	self->countdown = 50;
	self->state = Tusk_StateHurtFlash;
	return 0;
}
