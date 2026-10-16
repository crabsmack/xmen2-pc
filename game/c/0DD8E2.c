/* ControlBox_StateActivate - CONTROLBOX state once its switch is set: an explosion at its body
 * box's centre (Explosion_Spawn), no frame shown ($FF), its firing pattern started (class slot
 * +$6E, ControlBox_StartPattern); ControlBox_StateArm from the next update.
 *
 * self: the box. Returns self->flags & 8. */
#include "xmen2.h"

short ControlBox_StateActivate(register ControlBox *self)
{
	register long x = (self->boxRight + self->boxLeft) << 7;
	register long y = (self->boxTop + self->boxBottom) << 7;

	Explosion_Spawn(x, y, 0);
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	(*self->cls->method[26])(self);			/* +$6E ControlBox_StartPattern */
	SET_STATE(self, ControlBox_StateArm);
	return self->flags & 8;
}
