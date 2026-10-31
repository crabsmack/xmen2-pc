/* ExplodeFx_ChainHit - ObjList_ForEachInBox callback of a damaging chain explosion: the object
 * takes a hit (TakeDamage, no attacker, damage 1 at (x, y)): hit type 4 for a player (index 0 or
 * 1), $C for anything else.
 *
 * o: the object in the box.  x, y: the hit point (the explosion's top left). */
#include "xmen2.h"

void ExplodeFx_ChainHit(register Actor *o, register short x, register short y)
{
#if defined(XMEN2_NATIVE)
	/* phase 7 (R11): players 3-4 are players too (their objects aren't slots 0-1), hit in their context
	   as Collide_ObjVsList does; with $C they bounced off every damaging chain unhurt (SentinelsB2's fire
	   walls in the playtest of 2026-10-31: "wolverine (3p) became immune to the damage from the xplosions") */
	if (P34_IS_PLAYER(o)) {
		P34_HIT(o, (*o->cls->method[8])(o, 0, 4, 1, x, y));	/* +$26 TakeDamage */
		return;
	}
#endif
	if (o->index < 2)
		(*o->cls->method[8])(o, 0, 4, 1, x, y);		/* +$26 TakeDamage */
	else
		(*o->cls->method[8])(o, 0, 0xC, 1, x, y);	/* +$26 TakeDamage */
}
