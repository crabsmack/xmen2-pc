/* SavageMediumBio_OnDeath - SAVAGEMEDIUMBIO OnDeath (class slot +$72): it stops, leaves both
 * enemy hit lists (body, attack), flags bit 2 off, flags6 bits 0 and 1 on (not hittable, deals
 * no hits), and bursts from the next update (SavageMediumBio_StateBurst).
 *
 * self: the object. */
#include "xmen2.h"

void SavageMediumBio_OnDeath(register SavageMediumBio *self)
{
	STOP_MOVING(self);
	List_Unlink(self->bodyNode, &g_HitListEnemyBody);
	self->bodyNode = 0;
	List_Unlink(self->attackNode, &g_HitListEnemyAtk);
	self->attackNode = 0;
	self->flags &= ~4;
	self->flags6 |= 1;
	self->flags6 |= 2;
	self->state = SavageMediumBio_StateBurst;
	self->timer = 0;
}
