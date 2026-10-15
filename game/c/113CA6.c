/* XNpc_Attack - XNPC Attack (class slot +$22): hit a target through its TakeDamage, then wait
 * 30 frames before hitting again.
 *
 * self:   the attacker.
 * target: what it hits.
 * type:   hit type, passed on.
 * damage: -1: the stats record's attack damage.
 * hx, hy: the hit point, passed on.
 * (ACTOR's Attack waits 10 frames.) */
#include "xmen2.h"

void XNpc_Attack(XNpc *self, Actor *target, char type, char damage, short hx, short hy)
{
	(*target->cls->method[8])(target, self, type,	/* +$26 TakeDamage */
		damage == -1 ? self->stats->attack : damage, hx, hy);
	self->hitCooldown = 30;
}
