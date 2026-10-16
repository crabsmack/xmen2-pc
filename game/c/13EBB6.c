/* ExplodeFx_ChainHit - ObjList_ForEachInBox callback of a damaging chain explosion: the object
 * takes a hit (TakeDamage, no attacker, damage 1 at (x, y)): hit type 4 for a player (index 0 or
 * 1), $C for anything else.
 *
 * o: the object in the box.  x, y: the hit point (the explosion's top left). */
#include "xmen2.h"

void ExplodeFx_ChainHit(register Actor *o, register short x, register short y)
{
	if (o->index < 2)
		(*o->cls->method[8])(o, 0, 4, 1, x, y);		/* +$26 TakeDamage */
	else
		(*o->cls->method[8])(o, 0, 0xC, 1, x, y);	/* +$26 TakeDamage */
}
