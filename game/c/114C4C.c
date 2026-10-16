/* BioNpc_UpdateStunned - a stunned BIONPC's timer function (BioNpc_TakeDamage puts it in place of
 * Update): while stun frames are left, a stun step (BioNpc_StunShake; 2 frames in two-player
 * stepping). When they run out: the timer runs the class's Update (slot +$4A) again, the saved
 * palette comes back, and it dies (slot +$72 OnDeath(1, $D)) without health, else its draw-list
 * node draws it with the class's Draw (slot +$1A) again.
 *
 * self: the object (the timer's argument). */
#include "xmen2.h"

void BioNpc_UpdateStunned(register BioNpc *self)
{
	register DrawNode *node;

	if ((self->stun & 0x3FFF) > 0) {
		BioNpc_StunShake(self, g_TwoPlayerStep + 1);
		if (!((self->stun & 0x3FFF) > 0)) {
			((Timer *)self->timerObj)->func = (TimerFn)self->cls->method[17];	/* +$4A Update */
			self->spr.flags = self->spr.flags & ~3 | self->stun >> 14;
			if (self->health <= 0)
				(*self->cls->method[27])(self, 1, 0xD);	/* +$72 OnDeath */
			else if (self->drawNode >= 0) {
				node = &g_DrawNodes[self->drawNode];
				node->draw = (DrawFn)self->cls->method[5];	/* +$1A Draw */
			}
		}
	}
}
