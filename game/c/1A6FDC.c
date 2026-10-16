/* Nightcrawler_StateGrab - NIGHTCRAWLER climb state (class slot +$AA): take hold of what
 * XClimber_CheckGrab found (climbSeg, climb state in climbFlags bits 0-1).
 *
 * self: the player.
 * playerFlags 2 and 4 off, stateTimer and actionTimer 0, stopped, no animation, B unlatched, sound
 * $81 (tells the camera). Ceiling (1): the hanging pose (Tbl_Anim_1A5FEC), the box's top on the
 * ceiling's y2, then Nightcrawler_StateCeilingHang. Wall on the right (2): h-flipped, the wall
 * pose (Tbl_Anim_1A5FBE), flags6 $2000, the box's left on the wall's x2; wall on the left (3):
 * not flipped, the box's right on its x1; both then Nightcrawler_StateWallCling. All from the
 * next update.
 * Returns self->flags & 8; nothing with no climb state (it never happens). */
#include "xmen2.h"

short Nightcrawler_StateGrab(register Nightcrawler *self)
{
	register Seg *seg = self->climbSeg;

	self->playerFlags &= ~2;
	self->playerFlags &= ~4;
	g_CamModeSwitched = 1;
	self->stateTimer = 0;
	self->actionTimer = 0;
	STOP_MOVING(self);
	self->anim = 0;
	if (!self->pad)
		g_Pad1Latch = g_Pad1Latch & ~g_PadMaskB;
	else
		g_Pad2Latch = g_Pad2Latch & ~g_PadMaskB;
	Snd_Stop(0x81);
	Snd_Play(0x81);
	switch (self->climbFlags & 3) {
	case 1:
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FEC));
		Obj_UpdateBox(self);
		self->y += (seg->y2 - self->boxTop) << 8;
		SET_STATE(self, Nightcrawler_StateCeilingHang);
		return self->flags & 8;
	case 2:
		self->spr.flags |= 0x10;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
		Obj_UpdateBox(self);
		self->flags6 |= 0x2000;
		self->x += (seg->x2 - self->boxLeft) << 8;
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	case 3:
		self->spr.flags &= ~0x10;
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FBE));
		Obj_UpdateBox(self);
		self->flags6 |= 0x2000;
		self->x += (seg->x1 - self->boxRight) << 8;
		SET_STATE(self, Nightcrawler_StateWallCling);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
