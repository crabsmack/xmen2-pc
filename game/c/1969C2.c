/* Wolverine_StateWallCling - Wolverine clings to a wall (climbSeg): C leaps off it, Up/Down climb a
 * step (Wolverine_StateWallClimb), B slashes from it.
 *
 * self: the player.
 * C latched: no animation, the camera told (g_CamModeSwitched), the jump frame $31 with the box
 * refitted and its back edge put against the wall, turned away from it; vy -$834 (0 while
 * playerFlags $10000), the air speed and gravity of his record; the climb state, flags6 $2000
 * and the crawl-end marks (playerFlags $80000, $100000) cleared; the control state (slot +$A6)
 * from the next frame. Up latched, the box's top not above the wall's (Player_TopAboveSeg 0):
 * the other claw leads with its climb-up animation (Tbl_WolverineAnim2 / 3), vy -$226. Down
 * latched, the bottom not 40 px below it (Player_BottomBelowSeg): Tbl_WolverineAnim4 / 5, vy $2DA.
 * B latched (unlatched): ground-B strength, his attack sound, the wall slash
 * (Tbl_WolverineAnim8), clinging again at its end. Else stop in x and cling.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_StateWallCling(register Wolverine *self)
{
	register Seg *wall = self->climbSeg;

	if (!self->pad ? g_Pad1Latch & g_PadMaskC : g_Pad2Latch & g_PadMaskC) {
		self->anim = 0;
		g_CamModeSwitched = 1;
		(*self->cls->method[20])(self, 0x31);		/* SetFrame */
		Obj_UpdateBox(self);
		if (self->spr.flags & 0x10)
			self->x = (wall->x2 + (self->x >> 8) - self->boxLeft) << 8;
		else
			self->x = (wall->x1 + (self->x >> 8) - self->boxRight) << 8;
		self->spr.flags ^= 0x10;
		self->vy = !(self->playerFlags & 0x10000) ? -0x834 : 0;
		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		self->climbFlags = self->climbFlags & ~3 | 0;
		self->flags6 &= ~0x2000;
		self->playerFlags &= ~0x80000;
		self->playerFlags &= ~0x100000;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if ((!self->pad ? g_Pad1Latch & g_PadMaskUp : g_Pad2Latch & g_PadMaskUp)
	    && !Player_TopAboveSeg(self, 0, wall)) {
		if (!self->clawPhase) {
			self->clawPhase = 1;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim2));
		} else {
			self->clawPhase = 0;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim3));
		}
		self->vy = -0x226;
		SET_STATE(self, (StateFn)Wolverine_StateWallClimb);
		return self->flags & 8;
	}
	if ((!self->pad ? g_Pad1Latch & g_PadMaskDown : g_Pad2Latch & g_PadMaskDown)
	    && !Player_BottomBelowSeg(self, 0x28, wall)) {
		if (!self->clawPhase) {
			self->clawPhase = 1;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim4));
		} else {
			self->clawPhase = 0;
			Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim5));
		}
		self->vy = 0x2DA;
		SET_STATE(self, (StateFn)Wolverine_StateWallClimb);
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB) {
		if (!self->pad)
			g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
		else
			g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
		self->attack = self->charRec->strength[4];
		Snd_Stop(self->charRec->attackSound);
		Snd_Play(self->charRec->attackSound);
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim8));
		SET_STATE_WAIT(self, (StateFn)Wolverine_StateWallCling, 0xFF);
		return self->flags & 8;
	}
	self->ax = 0;
	self->vx = 0;
	SET_STATE(self, (StateFn)Wolverine_StateWallCling);
	return self->flags & 8;
}
