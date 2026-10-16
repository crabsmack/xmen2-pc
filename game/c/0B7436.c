/* ChildShip_OnHurt - CHILDSHIP OnHurt (class slot +$6E): knocked away by the hit.
 *
 * self: the object.  damage: the hit's damage (unused).  type: hit type: 9-12 the direction it
 * is knocked (9 left, 10 right, 11 down, 12 up), other types don't move it.
 * ChildShip_StateKnockback runs on the next update. Knocked at $500 with a deceleration of $C0
 * against the motion, in ax for both axes: a vertical knockback brakes x, not y (a bug, as in
 * CapedAcolyte_OnHurt). The cases are the fragments ChildShip_OnHurt_Up (12, and 11),
 * _Left (9), _Right (10) and the tail _SetDecel. */
#include "xmen2.h"

void ChildShip_OnHurt(ChildShip *self, char damage, char type)
{
	self->state = ChildShip_StateKnockback;
	self->timer = 0;
	switch (type) {
	case 12:			/* ChildShip_OnHurt_Up */
		self->vy = -0x500;
		break;
	case 11:
		self->vy = 0x500;
		break;
	case 9:				/* ChildShip_OnHurt_Left */
		self->vx = -0x500;
		break;
	case 10:			/* ChildShip_OnHurt_Right */
		self->vx = 0x500;
		break;
	}
	/* ChildShip_OnHurt_SetDecel */
	if (self->vx > 0)
		self->ax = -0xC0;
	else if (self->vx < 0)
		self->ax = 0xC0;
	if (self->vy > 0)
		self->ax = -0xC0;
	else if (self->vy < 0)
		self->ax = 0xC0;
}
