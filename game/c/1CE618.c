/* Magneto_GroundAttack - MAGNETO B on the ground (class slot +$D2): stop and aim
 * (Magneto_StateAim, now), standing (px, +$10, the crouch flag: 0).
 *
 * self: the player. */
#include "xmen2.h"

void Magneto_GroundAttack(Magneto *self)
{
	self->px = 0;
	self->ax = 0;
	self->vx = 0;
	GOTO_STATE(self, Magneto_StateAim);
}
