/* CloneA5_SegHook17 - floor-segment type hook for type $17, the boss's body (installed by
 * BroodBoss_StateWaitArrive, removed by BroodBoss_Defeat): in mode 3, an object whose body box
 * misses the segment's rectangle gets 1; a player inside it (not flashing) is hurt: TakeDamage
 * (class slot +$26) from no attacker, hit type 5, 2 points, at its box centre.
 *
 * mode: what the segment code asks.  seg: the segment.  obj: the object tested.
 * Returns 1 for a miss in mode 3, else 0. */
#include "xmen2.h"

short CloneA5_SegHook17(short mode, register Seg *seg, register Actor *obj)
{
	short cx;
	short cy;

	if (mode == 3) {
		if (obj->boxTop >= seg->y2 || obj->boxLeft >= seg->x2 || obj->boxRight < seg->x1
		    || obj->boxBottom <= seg->y1)
			return 1;
		if ((obj == (Actor *)&g_Player1 || obj == (Actor *)&g_Player2) && !obj->hitCooldown) {
			cx = (obj->boxLeft + obj->boxRight) >> 1;
			cy = (obj->boxTop + obj->boxBottom) >> 1;
			(*obj->cls->method[8])(obj, 0, 5, 2, cx, cy);	/* +$26 TakeDamage */
		}
	}
	return 0;
}
