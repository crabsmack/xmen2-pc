/* Beast_StateCling - Beast clings to a wall (Beast_StateGrabWall): C leaps off it, B swipes from
 * it.
 *
 * self: the player.
 * C pressed: no animation, the jump frame (9), vy -$834 (0 while playerFlags $10000), the air
 * speed and normal gravity of his record; the camera goes back to normal (g_CamModeSwitched 0),
 * the climb state is dropped and the control state (slot +$A6) follows. B latched: unlatched,
 * the attack strength of a ground B (strength[4]), his attack sound and the wall swipe
 * (Tbl_Anim_1829BE), clinging again at its end.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Beast_StateCling(register Beast *self)
{
	if (!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC) {
		self->anim = 0;
		(*self->cls->method[20])(self, 9);		/* SetFrame */
		self->vy = !(self->playerFlags & 0x10000) ? -0x834 : 0;
		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		g_CamModeSwitched = 0;
		self->climbFlags = self->climbFlags & ~3 | 0;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
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
		Obj_SetAnim(self, CODEPTR(Tbl_Anim_1829BE));
		SET_STATE_WAIT(self, (StateFn)Beast_StateCling, 0xFF);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Beast_StateCling);
	return self->flags & 8;
}
