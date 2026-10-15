/* Wolverine_StateGrab - WOLVERINE climbing state (class slot +$AA, after XClimber_CheckGrab): take
 * hold of the ceiling or wall he touched: hang (Wolverine_StateCeilingHang) or cling
 * (Wolverine_StateWallCling), from the next frame.
 *
 * self: the player (climbSeg: the segment; climbFlags & 3: 1 ceiling, 2/3 wall).
 * playerFlags 2, 4 and $20 cleared, flags6 $2000 (on a wall or ceiling), B unlatched; his attack
 * sound stops and the claw sound $63 plays; the camera is told (g_CamModeSwitched); stateTimer
 * and unk_A8 cleared, stop, no animation. Ceiling: the hang frame $30 (clawPhase 1), the box
 * refitted, the end marks (playerFlags $80000, $100000) cleared, his box's top to the ceiling.
 * Wall: facing away from it (h-flipped for 2), the cling frame $1B (clawPhase 0), the box refitted
 * and its front edge against the wall.
 * Returns self->flags & 8; nothing for another climb state. */
#include "xmen2.h"

short Wolverine_StateGrab(register Wolverine *self)
{
	register Seg *seg = self->climbSeg;

	self->playerFlags &= ~2;
	self->flags6 |= 0x2000;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	self->playerFlags &= ~4;
	self->playerFlags &= ~0x20;
	Snd_Stop(self->charRec->attackSound);
	Snd_Stop(0x63);
	Snd_Play(0x63);
	g_CamModeSwitched = 1;
	self->stateTimer = 0;
	self->unk_A8 = 0;
	STOP_MOVING(self);
	self->anim = 0;
	switch (self->climbFlags & 3) {
	case 1:
		(*self->cls->method[20])(self, 0x30);		/* SetFrame */
		self->clawPhase = 1;
		Obj_UpdateBox(self);
		self->playerFlags &= ~0x80000;
		self->playerFlags &= ~0x100000;
		self->y += (seg->y2 - self->boxTop) << 8;
		SET_STATE(self, (StateFn)Wolverine_StateCeilingHang);
		return self->flags & 8;
	case 2:
		self->spr.flags |= 0x10;
		(*self->cls->method[20])(self, 0x1B);
		self->clawPhase = 0;
		Obj_UpdateBox(self);
		self->x += (seg->x2 - self->boxLeft) << 8;
		SET_STATE(self, (StateFn)Wolverine_StateWallCling);
		return self->flags & 8;
	case 3:
		self->spr.flags &= ~0x10;
		(*self->cls->method[20])(self, 0x1B);
		self->clawPhase = 0;
		Obj_UpdateBox(self);
		self->x += (seg->x1 - self->boxRight) << 8;
		SET_STATE(self, (StateFn)Wolverine_StateWallCling);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
