/* Tusk_HurtCharge - TUSK hit by anything but hit types $D-$F (Tusk_TakeDamage): leave the enemy
 * body hit list, sound $E5, and charge after 50 frames (Tusk_StateChargeStart).
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_HurtCharge(Tusk *self)
{
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	self->hittable = 0;
	Snd_Stop(0xE5);
	Snd_Play(0xE5);
	self->state = Tusk_StateChargeStart;
	self->timer = 50;
	return 0;
}
