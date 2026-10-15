/* Gambit_CardHitEnemy - a card reaches an enemy body (ObjList_ForEachAtPoint callback of
 * Gambit_CardCollide): the card's damage of its hit type, and an explosion at the point.
 *
 * target: the object hit.  x, y: the point (pixels).  card: the card.
 * No attacker is named. The explosion takes the target's sprite priority (flags bit 2).
 * Returns 1: stop at this object. */
#include "xmen2.h"

char Gambit_CardHitEnemy(register Actor *target, register short x, register short y, void *data)
{
	register GambitCard *card = (GambitCard *)data;

	(*target->cls->method[8])(target, 0, card->hitType, card->damage, x, y);	/* +$26 TakeDamage */
	Explosion_Spawn(x << 8, y << 8, target->spr.flags & 4 ? 1 : 0);
	return 1;
}
