/* Wolverine_StartWallClimb - Wolverine, come round a ceiling's end onto a wall
 * (Wolverine_CeilingToWall), starts climbing down it (Wolverine_StateWallClimb, from the next
 * frame).
 *
 * self: the player (climbSeg: the wall; climbFlags & 3: 2 wall on his left, else on his right).
 * The camera is told (g_CamModeSwitched), stop, flags6 $2000 (on a wall), the climb animation
 * by clawPhase (Tbl_WolverineAnim12 / 13). Facing the wall, the box refitted, his box's front
 * against it; y moves by the change of the box's top, 10 px lower; vy $2DA.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_StartWallClimb(register Wolverine *self)
{
	register short top = self->boxTop;

	g_CamModeSwitched = 1;
	STOP_MOVING(self);
	self->flags6 |= 0x2000;
	if (!self->clawPhase)
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim12));
	else
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim13));
	if ((self->climbFlags & 3) == 2) {
		self->spr.flags |= 0x10;
		Obj_UpdateBox(self);
		self->x += (self->climbSeg->x2 - self->boxLeft) << 8;
		self->y = (top + (self->y >> 8) - self->boxTop + 10) << 8;
	} else {
		self->spr.flags &= ~0x10;
		Obj_UpdateBox(self);
		self->x += (self->climbSeg->x1 - self->boxRight) << 8;
		self->y = (top + (self->y >> 8) - self->boxTop + 10) << 8;
	}
	self->vy = 0x2DA;
	SET_STATE(self, (StateFn)Wolverine_StateWallClimb);
	return self->flags & 8;
}
