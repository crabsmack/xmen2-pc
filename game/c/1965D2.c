/* Wolverine_WallToCeiling - climbing up a wall, Wolverine reached a ceiling (Wolverine_StateWallClimb):
 * take hold of it and start crawling along it (Wolverine_StartCeilingCrawl, in 6 frames).
 *
 * self: the player.
 * The segment just touched (g_Segs[pushSeg]) becomes climbSeg and its kind (ClassifySeg, slot
 * +$F2) the climb state; the camera is told (g_CamModeSwitched); the corner frame $23 or $1E by
 * clawPhase; stop. The pixel x is computed and never used.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_WallToCeiling(register Wolverine *self)
{
	register short px;
	register Seg *seg;
	register short kind;

	px = kind = self->x >> 8;		/* (px is never used) */
	seg = self->climbSeg;
	seg = &g_Segs[self->pushSeg];
	kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
	self->climbSeg = seg;
	self->climbFlags = self->climbFlags & ~3 | kind;
	g_CamModeSwitched = 1;
	if (!self->clawPhase)
		(*self->cls->method[20])(self, 0x23);	/* SetFrame */
	else
		(*self->cls->method[20])(self, 0x1E);
	STOP_MOVING(self);
	SET_STATE_WAIT(self, (StateFn)Wolverine_StartCeilingCrawl, 6);
	return self->flags & 8;
}
