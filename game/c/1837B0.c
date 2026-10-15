/* Beast_SetFrameKeepCenterX - show another frame and move Beast so that the horizontal centre of
 * his body box stays where it was (Beast_SetFrameKeepCenter for x only).
 *
 * self:  the player.  frame: the frame (SetFrame, slot +$56).
 * The box is refitted (Obj_UpdateBox) and the camera told (g_CamModeSwitched). */
#include "xmen2.h"

void Beast_SetFrameKeepCenterX(register Beast *self, short frame)
{
	register long cx0, cx1;

	cx0 = (self->boxRight + self->boxLeft) << 7;
	(*self->cls->method[20])(self, frame);		/* SetFrame */
	Obj_UpdateBox(self);
	cx1 = (self->boxRight + self->boxLeft) << 7;
	self->x -= cx1 - cx0;
	g_CamModeSwitched = 1;
}
