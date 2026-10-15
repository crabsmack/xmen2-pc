/* Nightcrawler_WallToCeiling - NIGHTCRAWLER from a wall onto the ceiling just reached: the
 * ceiling pose (Tbl_Anim_1A5FEC), the box's top moved to the ceiling (climbSeg y2), then hang
 * (Nightcrawler_StateCeilingHang) from the next update.
 *
 * self: the player.
 * While the direction he climbed in is still held (Right when h-flipped, else Left) it first
 * waits for its release (Nightcrawler_StateWaitRightRelease / _StateWaitLeftRelease). Tells the
 * camera. The box top before the move is read and not used.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Nightcrawler_WallToCeiling(register Nightcrawler *self)
{
	register short top = self->boxTop;

	g_CamModeSwitched = 1;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1A5FEC));
	Obj_UpdateBox(self);
	self->y += (self->climbSeg->y2 - self->boxTop) << 8;
	if (self->spr.flags & 0x10) {
		if (!self->pad ? g_Pad1Held & g_PadMaskRight : g_Pad2Held & g_PadMaskRight) {
			SET_STATE(self, Nightcrawler_StateWaitRightRelease);
			return self->flags & 8;
		}
	} else if (!self->pad ? g_Pad1Held & g_PadMaskLeft : g_Pad2Held & g_PadMaskLeft) {
		SET_STATE(self, Nightcrawler_StateWaitLeftRelease);
		return self->flags & 8;
	}
	SET_STATE(self, Nightcrawler_StateCeilingHang);
	return self->flags & 8;
}
