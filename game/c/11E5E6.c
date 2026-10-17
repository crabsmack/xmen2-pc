/* BioBullet_SetVelocity - set a BIOBULLET's velocity and sprite priority (its shooter, right after
 * making it: XClone, BioSentinel).
 *
 * self: the bullet (its callers have it as the Actor New made).  vx, vy: velocity.
 * high: non-zero: high priority (flags bit 2). */
#include "xmen2.h"

void BioBullet_SetVelocity(Actor *self, short vx, short vy, char high)
{
	self->vx = vx;
	self->vy = vy;
	if (high)
		self->spr.flags |= 4;
	else
		self->spr.flags &= ~4;
}
