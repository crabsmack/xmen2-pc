/* Magneto_GroundAttackUp - MAGNETO Up+B on the ground (class slot +$D6): the same as B
 * (Magneto_GroundAttack): stop and aim (Magneto_StateAim, now), standing (px, +$10: 0).
 *
 * self: the player. */
#include "xmen2.h"

void Magneto_GroundAttackUp(Magneto *self)
{
	self->px = 0;
	self->ax = 0;
	self->vx = 0;
	GOTO_STATE(self, Magneto_StateAim);
}
