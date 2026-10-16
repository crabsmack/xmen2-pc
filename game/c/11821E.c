/* Explosion_HurtActor - a damaging explosion reaches an object (ObjList_ForEachInBox callback of
 * Explosion_Update): it takes 4 damage, hit type 4 (its TakeDamage, class slot +$26).
 *
 * target: the object.  x, y: the explosion's top-left (world pixels), passed as the hit point.
 * Returns what TakeDamage returned (ObjList_ForEachInBox stops at a non-zero low byte). */
#include "xmen2.h"

char Explosion_HurtActor(register Actor *target, short x, short y)
{
	return (char)(*target->cls->method[8])(target, 0, 4, 4, x, y);	/* +$26 TakeDamage */
}
