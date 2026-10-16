/* Wolverine_CeilingToWall - crawling along a ceiling, Wolverine came round its end onto a wall
 * (Wolverine_StateCeilingCrawl): take hold of the wall and climb it (Wolverine_StartWallClimb,
 * from the next frame).
 *
 * self: the player.
 * The segment just touched (g_Segs[pushSeg]) becomes climbSeg and its kind (ClassifySeg, slot
 * +$F2) the climb state; no animation; the camera is told (g_CamModeSwitched); the corner frame
 * $2B or $2E by clawPhase; stop. The pixel y is computed and never used.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_CeilingToWall(register Wolverine *self)
{
	register short py;
	register Seg *seg;
	register short kind;

	py = kind = self->y >> 8;		/* (py is never used) */
	seg = &g_Segs[self->pushSeg];
	kind = (*self->cls->method[59])(self, seg);	/* ClassifySeg */
	self->anim = 0;
	self->climbSeg = seg;
	self->climbFlags = self->climbFlags & ~3 | kind;
	g_CamModeSwitched = 1;
	if (!self->clawPhase)
		(*self->cls->method[20])(self, 0x2B);	/* SetFrame */
	else
		(*self->cls->method[20])(self, 0x2E);
	STOP_MOVING(self);
	SET_STATE(self, (StateFn)Wolverine_StartWallClimb);
	return self->flags & 8;
}
