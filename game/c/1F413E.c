/* XClimber_PlaceAtSafePos - XCLIMBER PlaceAtSafePos (class slot +$9E): bring the player back to its
 * safe spot (XClimber_SaveSafePos): climbing there, the saved segment is held again.
 *
 * self: the player.
 * Saved climbing (climbFlags bits 4-5): that climb state, climbSeg = safeSeg, no wall jump
 * queued (climbFlags $40), and the climbing state (slot +$AA) next. Else XPLAYER's
 * PlaceAtSafePos (XPlayer_PlaceAtSafePos) becomes the state. The other player is looked up and
 * never used. */
#include "xmen2.h"

void XClimber_PlaceAtSafePos(register XClimber *self)
{
	register XPlayer *other = (XPlayer *)self == &g_Player1 ? &g_Player2 : &g_Player1;
	register short climb;

	climb = (self->climbFlags & 0x30) >> 4;
	if (climb) {
		self->climbFlags = self->climbFlags & ~3 | climb;
		self->climbSeg = self->safeSeg;
		self->climbFlags &= ~0x40;
		self->state = (StateFn)self->cls->method[41];		/* the climbing state */
	} else
		self->state = (StateFn)CLASS(Tbl_XClimber_vtable)->parent->method[38];	/* XPLAYER PlaceAtSafePos */
	if (0)
		(void)other;
}
