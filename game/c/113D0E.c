/* XNpc_GetMoveLimit - XNPC class slot +$7A: how far it may be knocked in a direction
 * (XNpc_HurtReact / XNpc_DeathReact stop the knockback there): no limit.
 *
 * self: the object.  dir: 0 left, 1 right, 2 up, 3 down.
 * *kind = 2 (no limit); returns 0. Subclasses with a range override it (BabyBio_GetMoveLimit). */
#include "xmen2.h"

short XNpc_GetMoveLimit(XNpc *self, char dir, u8 *kind)
{
	*kind = 2;
	return 0;
}
