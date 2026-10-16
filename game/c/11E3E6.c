/* BioBullet_HitActor - a BIOBULLET reaches a player's body (ObjList_ForEachAtPoint callback of
 * BioBullet_Update): 1 damage of hit type 8 there, no attacker named.
 *
 * target: the object hit.  x, y: the point (pixels).
 * Returns 1: stop at this object. */
#include "xmen2.h"

char BioBullet_HitActor(Actor *target, short x, short y)
{
	(*target->cls->method[8])(target, 0, 8, 1, x, y);	/* +$26 TakeDamage */
	return 1;
}
