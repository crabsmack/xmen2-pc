/* FarAcolyte_StateWaitOnScreen - FARACOLYTE first state: invisible (frame $FF) until its anchor
 * is within 120 px (x) and 100 px (y) of the screen centre, then it appears
 * (FarAcolyte_StateAppear from the next frame).
 *
 * self: the acolyte.
 * On appearing: home x = the camera's left edge (it glides in from there, FarAcolyte_Move),
 * flags6 bit $4000 off. Returns self->flags & 8 then, nothing while it waits. */
#include "xmen2.h"

short FarAcolyte_StateWaitOnScreen(register FarAcolyte *self)
{
	register short cx, cy;

	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	cx = g_CameraX + 0xA0;
	cy = g_CameraY + 0x70;
	if (ABS(cx - self->px) < 0x78 && ABS(cy - self->py) < 0x64) {
		self->homeX = (long)g_CameraX << 8;
		self->flags6 &= ~0x4000;
		SET_STATE(self, FarAcolyte_StateAppear);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
