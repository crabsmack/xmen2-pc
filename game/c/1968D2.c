/* Wolverine_StartCeilingCrawl - Wolverine, holding a ceiling (Wolverine_WallToCeiling), turns round
 * and starts crawling along it (Wolverine_StateCeilingCrawl, from the next frame).
 *
 * self: the player (climbSeg: the ceiling).
 * Stop, flags6 $2000 and the crawl-end marks (playerFlags $80000, $100000) cleared; the camera is
 * told (g_CamModeSwitched). He turns round (h-flip), the other claw leads with its crawl
 * animation (Tbl_WolverineAnim14 / 15); the box refitted, his box's top goes to the ceiling and
 * x keeps the box's former front edge (left facing right before the turn); then vx $208 the way
 * he now faces.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_StartCeilingCrawl(register Wolverine *self)
{
	register short edge;

	g_CamModeSwitched = 1;
	STOP_MOVING(self);
	self->flags6 &= ~0x2000;
	self->playerFlags &= ~0x80000;
	self->playerFlags &= ~0x100000;
	if (self->spr.flags & 0x10)
		edge = self->boxLeft;
	else
		edge = self->boxRight;
	self->spr.flags ^= 0x10;
	if (!self->clawPhase) {
		self->clawPhase = 1;
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim14));
	} else {
		self->clawPhase = 0;
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim15));
	}
	Obj_UpdateBox(self);
	self->y += (self->climbSeg->y2 - self->boxTop) << 8;
	if (self->spr.flags & 0x10) {
		self->x -= (self->boxRight - edge) << 8;
		self->vx = -0x208;
	} else {
		self->x -= (self->boxLeft - edge) << 8;
		self->vx = 0x208;
	}
	SET_STATE(self, (StateFn)Wolverine_StateCeilingCrawl);
	return self->flags & 8;
}
