/* XPlayer_EnterAirFrame - switch to the frame for the speed in the air (class slot +$7E) and move
 * the player so that its feet and its front edge stay where they were.
 *
 * self: the player.
 * Stops the animation; the box is refitted (Obj_UpdateBox) and x moves by the change of the
 * box's front (left facing right, right when h-flipped), y by the change of its bottom.
 * Tells the camera (g_CamModeSwitched). */
#include "xmen2.h"

void XPlayer_EnterAirFrame(XPlayer *self)
{
	short front;
	register short bottom;

	bottom = self->boxBottom;
	self->anim = 0;
	if (!(self->spr.flags & 0x10)) {
		front = self->boxLeft;
		(*self->cls->method[30])(self);		/* SetAirFrame */
		Obj_UpdateBox(self);
		self->x += (front - self->boxLeft) << 8;
	} else {
		front = self->boxRight;
		(*self->cls->method[30])(self);		/* SetAirFrame */
		Obj_UpdateBox(self);
		self->x += (front - self->boxRight) << 8;
	}
	self->y += (bottom - self->boxBottom) << 8;
	g_CamModeSwitched = 1;
}
