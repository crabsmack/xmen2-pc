/* ObjList_ForEachInBox - run func on each object of a hit list whose second box overlaps the
 * box (x1, y1)-(x2, y2), until one returns non-zero.
 *
 * x1, y1, x2, y2: the box (world pixels, inclusive).
 * list:  first node of the hit list (g_HitListEnemyBody, g_HitListPlayerBody; node owner = object).
 * func:  an ObjListFn passed as data: func(object, x1, y1) (Explosion_HurtActor, ExplodeFx_ChainHit).
 * all:   non-zero: objects still flashing from a hit (hitCooldown) count too.
 * Returns 1 when func stopped the walk or any object overlapped, else 0. */
#include "xmen2.h"

char ObjList_ForEachInBox(register short x1, register short y1, register short x2, register short y2,
	ListNode *list, void *func, unsigned char all)
{
	register Actor *o;
	char hit = 0;

	while (list) {
		o = list->owner;
		if (x1 <= o->box2Right && x2 >= o->box2Left && y1 <= o->box2Bottom && y2 >= o->box2Top
		    && (all || !o->hitCooldown)) {
			hit = 1;
			if (FN_AT(ObjListFn, func)(o, x1, y1))
				return 1;
		}
		list = list->next;
	}
	return hit;
}
