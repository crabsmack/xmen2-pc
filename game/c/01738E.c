/* FarAcolyte_OutsideActiveWindow - FARACOLYTE class slot +$42: is it outside the active window?
 *
 * self: the acolyte.
 * With no orb or bolt out, the parent's (ACTOR) test. Otherwise the box spanning the acolyte,
 * its orb and its bolt (each one out; positions in pixels) is tested against the window
 * g_DeactWin* (screen coordinates): returns 1 when it lies wholly outside, else 0. */
#include "xmen2.h"

long FarAcolyte_OutsideActiveWindow(register FarAcolyte *self)
{
	short bx, by;
	short ox, oy;
	short top;
	register short x, y;
	register short left, right, bottom;

	x = self->x >> 8;
	y = self->y >> 8;
	if (!(self->shotFlags & 5))
		return (*CLASS(Tbl_FarAcolyte_vtable)->parent->method[15])(self);	/* ACTOR */
	if (self->shotFlags & 1) {
		ox = self->orb.x >> 8;
		oy = self->orb.y >> 8;
	} else {
		ox = x;
		oy = y;
	}
	if (self->shotFlags & 4) {
		bx = self->bolt.x >> 8;
		by = self->bolt.y >> 8;
	} else {
		bx = x;
		by = y;
	}
	left = bx < ox ? bx : ox;
	left = left < x ? left : x;
	right = bx > ox ? bx : ox;
	right = right > x ? right : x;
	top = by < oy ? by : oy;
	top = top < y ? top : y;
	bottom = by > oy ? by : oy;
	bottom = bottom > y ? bottom : y;
	left -= g_CameraX;
	right -= g_CameraX;
	top -= g_CameraY;
	bottom -= g_CameraY;
	if (left <= g_DeactWinRight && right >= g_DeactWinLeft
	    && top <= g_DeactWinBottom && bottom >= g_DeactWinTop)
		return 0;
	return 1;
}
