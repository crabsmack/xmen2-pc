/* Beast_SetFrameKeepCenter - show another frame and move Beast so that the centre of his body box
 * stays where it was.
 *
 * self:  the player.  frame: the frame (SetFrame, slot +$56).
 * The centres are kept with 7 fraction bits (half the box sums << 8); the box is refitted
 * (Obj_UpdateBox) and the camera told (g_CamModeSwitched). */
#include "xmen2.h"

void Beast_SetFrameKeepCenter(register Beast *self, short frame)
{
	register long cx0, cy0, cx1, cy1;

	cx0 = (self->boxRight + self->boxLeft) << 7;
	cy0 = (self->boxTop + self->boxBottom) << 7;
	(*self->cls->method[20])(self, frame);		/* SetFrame */
	Obj_UpdateBox(self);
	cx1 = (self->boxRight + self->boxLeft) << 7;
	cy1 = (self->boxTop + self->boxBottom) << 7;
	self->x -= cx1 - cx0;
	self->y -= cy1 - cy0;
	g_CamModeSwitched = 1;
}
