/* TechWallGun_SetupBurst - get a TECHWALLGUN's next burst ready: its shot animation, the aiming
 * state to go back to, the bullets' direction, a full burst of shots.
 *
 * self: the gun.  anim: animation of each shot.  aim: state after the burst.
 * dir:  bullet direction (1-9, TechWallGun_Fire). */
#include "xmen2.h"

void TechWallGun_SetupBurst(TechWallGun *self, void *anim, StateFn aim, short dir)
{
	self->fireAnim = anim;
	self->shotDir = dir;
	self->aimState = aim;
	self->shotsLeft = self->burstShots;
}
