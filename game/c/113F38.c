/* XNpc_OnDeath - XNPC OnDeath (class slot +$72, XNpc_TakeDamage calls it when a hit takes the
 * last health): the death reaction (XNpc_DeathReact).
 *
 * self: the object.  damage: the hit's damage (the knockback strength).  type: hit type. */
#include "xmen2.h"

void XNpc_OnDeath(XNpc *self, char damage, char type)
{
	XNpc_DeathReact(self, damage, type);
}
