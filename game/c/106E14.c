/* Platform1_MemberInWindow - PLATFORM1's class slot +$6E (SYNCHRONIZEDACTOR: is this member in
 * the activation window?): does the rectangle of its floor segment's ends, relative to the camera,
 * meet the activate window (g_ActWinLeft..g_ActWinBottom)?
 *
 * self: the platform. Returns 1 if so, else 0. */
#include "xmen2.h"

char Platform1_MemberInWindow(Platform1 *self)
{
	register Seg *seg = self->seg;
	register short x1 = seg->x1 - g_CameraX;
	register short y1 = seg->y1 - g_CameraY;
	register short x2 = seg->x2 - g_CameraX;
	register short y2 = seg->y2 - g_CameraY;
	short t;

	if (x1 > x2) {
		t = x1;
		x1 = x2;
		x2 = t;
	}
	if (y1 > y2) {
		t = y1;
		y1 = y2;
		y2 = t;
	}
	return x1 <= g_ActWinRight && x2 >= g_ActWinLeft && y1 <= g_ActWinBottom && y2 >= g_ActWinTop;
}
