/* Wolverine_StateCeilingHang - Wolverine hangs from a ceiling (climbSeg): C drops off, Left/Right
 * crawl a step along it (Wolverine_StateCeilingCrawl) unless that end is reached.
 *
 * self: the player.
 * C latched: the air speed and gravity of his record, the camera told (g_CamModeSwitched), the
 * climb state, flags6 $2000 and the end marks cleared, the control state (slot +$A6) from the
 * next frame. Right held: at the right end (playerFlags $100000) stop; else face right, vx $208,
 * the left end's mark cleared. Left the same the other way ($80000, vx -$208). Nothing held:
 * stop. Moving with his front not past the ceiling's end (Player_FrontPastSeg 0): the other claw
 * leads with its crawl animation (Tbl_WolverineAnim14 / 15) and the crawl state. Else stop and
 * hang.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_StateCeilingHang(register Wolverine *self)
{
	register Seg *ceiling = self->climbSeg;

	if (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC) {
		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		g_CamModeSwitched = 1;
		self->climbFlags = self->climbFlags & ~3 | 0;
		self->flags6 &= ~0x2000;
		self->playerFlags &= ~0x80000;
		self->playerFlags &= ~0x100000;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
		if (self->playerFlags & 0x100000) {
			self->ax = 0;
			self->vx = 0;
		} else {
			self->spr.flags &= ~0x10;
			self->vx = 0x208;
			self->playerFlags &= ~0x80000;
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		if (self->playerFlags & 0x80000) {
			self->ax = 0;
			self->vx = 0;
		} else {
			self->spr.flags |= 0x10;
			self->vx = -0x208;
			self->playerFlags &= ~0x100000;
		}
	} else {
		self->ax = 0;
		self->vx = 0;
	}
	if (self->vx && !Player_FrontPastSeg(self, 0, ceiling)) {
		if (!self->clawPhase) {
			self->clawPhase = 1;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim14));
		} else {
			self->clawPhase = 0;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim15));
		}
		SET_STATE(self, (StateFn)Wolverine_StateCeilingCrawl);
		return self->flags & 8;
	}
	self->ax = 0;
	self->vx = 0;
	SET_STATE(self, (StateFn)Wolverine_StateCeilingHang);
	return self->flags & 8;
}
