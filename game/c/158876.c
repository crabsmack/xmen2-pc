/* FluxBomb_DamageArea - a flux bomb's blast: every body of the other side's body list (the
 * enemies' for the player's bomb, flags6 $10: hit type 12; else the players': type 4) not
 * flashing whose second box meets the square around the bomb (65 px each way, 160 when strong)
 * gets the bomb's Attack (class slot +$22) with 2 damage (4 when strong); then the map-tile hooks
 * in that square run (FluxBomb_HitTilesInArea).
 *
 * self: the bomb (FluxBomb_StartBurst). */
#include "xmen2.h"

void FluxBomb_DamageArea(FluxBomb *self)
{
	short cx = self->x >> 8;
	short cy = self->y >> 8;
	short type;
	short damage;
	register short left;
	register short right;
	register short top;
	register short bottom;
	register ListNode *n;
	register Actor *o;

	if (self->strong) {
		left = cx - 0xA0;
		right = cx + 0xA0;
		top = cy - 0xA0;
		bottom = cy + 0xA0;
		damage = 4;
	} else {
		left = cx - 0x41;
		right = cx + 0x41;
		top = cy - 0x41;
		bottom = cy + 0x41;
		damage = 2;
	}
	if (self->flags6 & 0x10) {
		n = g_HitListEnemyBody;
		type = 12;
	} else {
		n = g_HitListPlayerBody;
		type = 4;
	}
	for (; n; n = n->next) {
		o = n->owner;
		if (o->hitCooldown)
			continue;
		if (left > o->box2Right || right < o->box2Left || top > o->box2Bottom || bottom < o->box2Top)
			continue;
		(*self->cls->method[7])(self, o, type, damage, -1, -1);	/* +$22 Attack */
	}
	FluxBomb_HitTilesInArea(self, left, top, right, bottom);
}
