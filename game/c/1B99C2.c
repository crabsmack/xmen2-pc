/* Psylocke_StateCling - Psylocke clings to a wall (Psylocke_StateGrabWall): C leaps off it, B
 * slashes from it.
 *
 * self: the player.
 * C pressed: no animation, the jump frame ($11), vy -$834 (0 while playerFlags $10000), the air
 * speed and normal gravity of her record; the camera goes back to normal (g_CamModeSwitched 0),
 * the climb state is dropped and the control state (slot +$A6) follows. B latched (and left
 * latched): sound $92, the attack strength of a ground B (strength[4]), hits of type 10 facing
 * right, 9 left, and the wall slash (Tbl_PsylockeAnim12_2), clinging again at its end. Else the
 * cling animation (Tbl_PsylockeAnim11_2, kept running).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Psylocke_StateCling(register Psylocke *self)
{
	if (!self->pad ? g_Pad1Pressed & g_PadMaskC : g_Pad2Pressed & g_PadMaskC) {
		self->anim = 0;
		(*self->cls->method[20])(self, 0x11);		/* SetFrame */
		self->vy = !(self->playerFlags & 0x10000) ? -0x834 : 0;
		self->maxVx = self->charRec->airMaxVx;
		self->ay = self->charRec->gravity;
		g_CamModeSwitched = 0;
		self->climbFlags = self->climbFlags & ~3 | 0;
		SET_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
		return self->flags & 8;
	}
	if (!self->pad ? g_Pad1Latch & g_PadMaskB : g_Pad2Latch & g_PadMaskB) {
		Snd_Stop(0x92);
		Snd_Play(0x92);
		self->attack = self->charRec->strength[4];
		self->hitType = !(self->spr.flags & 0x10) ? 10 : 9;
		Obj_SetAnim(self, CODEPTR(Tbl_PsylockeAnim12_2));
		SET_STATE_WAIT(self, (StateFn)Psylocke_StateCling, 0xFF);
		return self->flags & 8;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_PsylockeAnim11_2));
	SET_STATE(self, (StateFn)Psylocke_StateCling);
	return self->flags & 8;
}
