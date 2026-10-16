/* XClimber_Init - XCLIMBER constructor (class slot +$12): XPLAYER's Init (XPlayer_Init), then no
 * segment held and no climb state.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

void XClimber_Init(register XClimber *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_XClimber_vtable)->parent->method[3])(self, x, y, pl);	/* XPLAYER Init */
	self->climbSeg = 0;
	self->climbFlags = 0;
}
