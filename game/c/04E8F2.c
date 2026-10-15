/* ApocBomb_Attack - APOCBOMB Attack (class slot +$22): hit the target (its TakeDamage with the
 * same arguments, no hit cooldown), then blow up: one live bomb less, ExplodeFx_BurstSmall.
 *
 * self: the bomb.  target: what it touched.  type, damage: the hit.  hx, hy: the hit point. */
#include "xmen2.h"

#define g_ApocBombCount	(*(s16 *)g_SceneVar3B08)	/* scene variable: live bombs */

void ApocBomb_Attack(register ApocBomb *self, Actor *target, char type, char damage, short hx, short hy)
{
	(*target->cls->method[8])(target, self, type, damage, hx, hy);	/* +$26 TakeDamage */
	g_ApocBombCount--;
	ExplodeFx_BurstSmall(self);
}
