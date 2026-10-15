/* Tusk_Update - TUSK Update (class slot +$4A): the parent's (ACTOR); then, unless flags bit $10
 * is set: count down the frames it stays hittable (2 per frame in the two-player step; at 0 it
 * leaves the enemy body hit list), end the blink once the hit cooldown is out, and give each
 * live player low priority (sprite priority bit off, draw-list priority 0) while it is above
 * TUSK's home y, high priority (bit on, priority 1) otherwise; only on a change.
 *
 * self: TUSK.
 * The players are visited by a loop over g_Player1 and g_Player2. */
#include "xmen2.h"

void Tusk_Update(Tusk *self)
{
	register XPlayer *p;

	(*CLASS(Tbl_Tusk_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->flags & 0x10)
		return;
	if (self->hittable) {
		short n = g_TwoPlayerStep ? 2 : 1;

		do {
			self->hittable--;
			if (!self->hittable) {
				List_Unlink(self->bodyNode, &g_HitListEnemyBody);
				self->bodyNode = 0;
				break;
			}
		} while (--n);
	}
	if (!self->hitCooldown)
		self->flags6 &= ~0x40;
	{
		long y = self->py << 8;

		p = &g_Player1;
		do {
			if (IS_ALIVE(p)) {
				if (y > p->y) {
					if (p->spr.flags & 4) {
						p->spr.flags &= ~4;
						DrawList_SetPriority(p->drawNode, 0, p->layer, p->flags6 & 8);
					}
				} else if (!(p->spr.flags & 4)) {
					p->spr.flags |= 4;
					DrawList_SetPriority(p->drawNode, 1, p->layer, p->flags6 & 8);
				}
			}
		} while (p == &g_Player1 ? (p = &g_Player2) : 0);
	}
}
