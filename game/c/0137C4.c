/* CapedAcolyte_StateKnockback - CAPEDACOLYTE hurt state (set by CapedAcolyte_OnHurt): start a
 * 15-frame knockback (shake), CapedAcolyte_StateKnockbackRun from the next update.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short CapedAcolyte_StateKnockback(register CapedAcolyte *self)
{
	self->shake = 15;
	SET_STATE(self, CapedAcolyte_StateKnockbackRun);
	return self->flags & 8;
}
