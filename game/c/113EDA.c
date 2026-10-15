/* XNpc_OnHurt - XNPC OnHurt (class slot +$6E, XNpc_TakeDamage calls it when the object survives a
 * hit): the hurt reaction (XNpc_HurtReact).
 *
 * self: the object.  damage: the hit's damage (the knockback strength).  type: hit type. */
#include "xmen2.h"

void XNpc_OnHurt(XNpc *self, char damage, char type)
{
	XNpc_HurtReact(self, damage, type);
}
