/* Beast_ShockwaveHitCb - a shockwave reaches an object (ObjList_ForEachAtPoint callback of
 * Beast_ShockwaveStep): 2 damage of the wave's hit type, and an explosion at the point.
 *
 * target: the object hit.  x, y: the point (pixels).
 * type:   the hit type (TakeDamage's: 10 / 9 for enemies by the wave's side, 4 for players:
 *         Beast_ShockwaveStep).
 * No attacker is named. The explosion takes the target's sprite priority (flags bit 2).
 * Returns 0: go on with the other objects at the point. */
#include "xmen2.h"

char Beast_ShockwaveHitCb(register Actor *target, register short x, register short y, short type)
{
	(*target->cls->method[8])(target, 0, type, 2, x, y);	/* TakeDamage */
	Explosion_Spawn(x << 8, y << 8, target->spr.flags & 4 ? 1 : 0);
	return 0;
}
