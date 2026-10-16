/* SegHook_HurtPlayerStop - floor-segment type hook (scenes install it in g_SegTypeHooks): on
 * touch (mode 0) the segment stops the object (flags $100, clears the caller's flag 8) and hurts
 * a player who is not flashing: TakeDamage (class slot +$26) from no attacker, hit type 4,
 * 1 point, at (0, 0).
 *
 * mode: what the segment code asks (0: touch).  seg: the segment.  obj: the object.
 * flags: the segment code's result flags. Returns 0. */
#include "xmen2.h"

short SegHook_HurtPlayerStop(short mode, Seg *seg, register Actor *obj, s16 *flags)
{
	if (!mode) {
		*flags &= ~8;
		obj->flags |= 0x100;
		if ((obj == (Actor *)&g_Player1 || obj == (Actor *)&g_Player2) && !obj->hitCooldown)
			(*obj->cls->method[8])(obj, 0, 4, 1, 0, 0);	/* +$26 TakeDamage */
		return 0;
	}
	return 0;
}
