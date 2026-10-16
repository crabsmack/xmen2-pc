/* SentinelsB_HazardSeg - a Sentinels floor-segment type hook (type 21, SentinelsB2_InstallSegHooks):
 * a segment that hurts. For message 3, an object whose second box meets the segment's box is hit
 * when it is a player not in hit cooldown: 1 damage (TakeDamage, no attacker), hit type 6 when the
 * box reaches past the segment's right end, else 5 past its left end, else 4.
 *
 * msg: the hook's message.  seg: the segment.  obj: the object touching it.
 * Returns 0 when the boxes meet (hit or not), 1 when they don't; 0 for other messages. */
#include "xmen2.h"

short SentinelsB_HazardSeg(short msg, register Seg *seg, register Actor *obj)
{
	register char hitType;

	if (msg == 3) {
		if (obj->box2Top >= seg->y2 || obj->box2Left >= seg->x2 || obj->box2Right < seg->x1
		    || obj->box2Bottom <= seg->y1)
			return 1;
		hitType = 4;
		if (obj->box2Right > seg->x2)
			hitType = 6;
		else if (obj->box2Left > seg->x1)
			hitType = 5;
		if ((obj == (Actor *)&g_Player1 || obj == (Actor *)&g_Player2) && !obj->hitCooldown)
			(*obj->cls->method[8])(obj, 0, hitType, 1, 0, 0);	/* +$26 TakeDamage */
		return 0;
	}
	return 0;
}
