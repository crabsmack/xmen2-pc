/* ObjList_ForEachAtPoint - run func on each object of a hit list whose second box holds the point
 * (x, y), until one returns non-zero.
 *
 * x, y:  the point (world pixels).
 * list:  first node of the hit list (node owner = object).
 * func:  an ObjListFn passed as data: func(object, x, y, arg) (Beast_ShockwaveHitCb,
 *        Gambit_CardHitEnemy...).
 * all:   non-zero: objects still flashing from a hit (hitCooldown) count too.
 * arg:   passed on to func.
 * Returns 1 when func stopped the walk or any object held the point, else 0. */
#include "xmen2.h"

char ObjList_ForEachAtPoint(register short x, register short y, ListNode *list, void *func,
	unsigned char all, long arg)
{
	register Actor *o;
	register char hit = 0;

	while (list) {
		o = list->owner;
		if (x >= o->box2Left && x <= o->box2Right && y >= o->box2Top && y <= o->box2Bottom
		    && (all || !o->hitCooldown)) {
			hit = 1;
			if (FN_AT(ObjListFn, func)(o, x, y, arg))
				return 1;
		}
		list = list->next;
	}
	return hit;
}
