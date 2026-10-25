/* SegHook_HurtPlayer - floor-segment type hook (CloneA2_InstallHooks: type $16): on touch
 * (mode 0) clear the caller's flag 8 and hurt a player who is not flashing: TakeDamage (class
 * slot +$26) from no attacker, hit type 4, 1 point, at (0, 0). Like SegHook_HurtPlayerStop
 * without stopping the object.
 *
 * mode: what the segment code asks (0: touch).  seg: the segment.  obj: the object.
 * flags: the segment code's result flags. Returns 1 on touch, else 0. */
#include "xmen2.h"

short SegHook_HurtPlayer(short mode, Seg *seg, register Actor *obj, s16 *flags)
{
	if (!mode) {
		*flags &= ~8;
		if ((obj == (Actor *)&g_Player1 || obj == (Actor *)&g_Player2
#if defined(XMEN2_NATIVE)
		     || P34_IS_PLAYER(obj)	/* players 3-4 too (R11); their own code runs this, in their context */
#endif
		    ) && !obj->hitCooldown)
			(*obj->cls->method[8])(obj, 0, 4, 1, 0, 0);	/* +$26 TakeDamage */
		return 1;
	}
	return 0;
}
