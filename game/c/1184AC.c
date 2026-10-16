/* ICandy_Init - ICANDY Init (class slot +$0E): at (x, y), not moving, no animation, an empty
 * sprite instance (no definition, no frame uploaded, not attached), layer $7F, and its own timer
 * (period 1) running the class's Update (slot +$12) with the object as argument; ICandy_New starts
 * it.
 *
 * self: the object.  x, y: position (8 fraction bits).
 * The sprite definition is cleared twice. */
#include "xmen2.h"

void ICandy_Init(register ICandy *self, register long x, register long y)
{
	self->x = x;
	self->px = (short)(x >> 8);
	self->y = y;
	self->py = (short)(y >> 8);
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->ay = 0;
	self->flags = 0;
	self->anim = 0;
	self->sprFlags = 0;
	self->unk_27 = 0;
	self->frame = 0;
	self->sprDef = 0;
	self->uploaded = 0xFF;
	self->tileBase = -1;
	self->unk_3A = 0;
	self->unk_29 = 0xFF;
	self->unk_28 = 0xFF;
	self->unk_2A = 0xFF;
	self->unk_2B = 0xFF;
	self->sprDef = 0;
	self->unk_2C = 0;
	self->layer = 0x7F;
	self->timer = Timer_Create((TimerFn)self->cls->method[3], 1, self, 1);	/* +$12 Update */
}
